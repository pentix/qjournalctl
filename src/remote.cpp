#include "remote.h"
#include "passworddialog.h"
#include "exceptions.h"

#include <QProcess>
#include <QDebug>

Remote::Remote(QObject *qObject, SSHConnectionSettings *sshSettings)
{
    ssh = ssh_new();
    assert(ssh != nullptr);

    ssh_options_set(ssh, SSH_OPTIONS_HOST, sshSettings->getHostname());
    ssh_options_set(ssh, SSH_OPTIONS_USER, sshSettings->getUsername());
    ssh_options_set(ssh, SSH_OPTIONS_PORT, sshSettings->getPort());
    int ok;

    ok = ssh_connect(ssh);
    if(ok != SSH_OK){
        ssh_free(ssh);
        throw new Error("Establishing a connection to the remote host failed. Please try again!");
    }

    // Todo: Provide better case destinction
    ok = ssh_session_is_known_server(ssh);
    switch(ok) {
    case SSH_KNOWN_HOSTS_OK:
    {
        // Nothing happens
        break;
    }

    // Remote server is not in known_hosts file
    case SSH_KNOWN_HOSTS_NOT_FOUND:
    case SSH_KNOWN_HOSTS_UNKNOWN:
    {
        // Get the server's public key
        ssh_key serverPublicKey;
        ssh_get_server_publickey(ssh, &serverPublicKey);

        // Calculate fingerprints
        unsigned char *bufferMD5, *bufferSHA1, *bufferSHA256;
        size_t lengthMD5, lengthSHA1, lengthSHA256;
        ssh_get_publickey_hash(serverPublicKey, SSH_PUBLICKEY_HASH_MD5, &bufferMD5, &lengthMD5);
        ssh_get_publickey_hash(serverPublicKey, SSH_PUBLICKEY_HASH_SHA1, &bufferSHA1, &lengthSHA1);
        ssh_get_publickey_hash(serverPublicKey, SSH_PUBLICKEY_HASH_SHA256, &bufferSHA256, &lengthSHA256);

        // Get hex digest
        char *hexMD5 = ssh_get_hexa(bufferMD5, lengthMD5);
        char *hexSHA1 = ssh_get_hexa(bufferSHA1, lengthSHA1);
        char *hexSHA256 = ssh_get_hexa(bufferSHA256, lengthSHA256);

        QString hashes = "MD5:    " + QString(hexMD5) + "\n" +
                         "SHA1:   " + QString(hexSHA1) + "\n" +
                         "SHA256: " + QString(hexSHA256) + "\n";

        // Free library memory
        ssh_string_free_char(hexMD5);
        ssh_string_free_char(hexSHA1);
        ssh_string_free_char(hexSHA256);
        ssh_clean_pubkey_hash(&bufferMD5);
        ssh_clean_pubkey_hash(&bufferSHA1);
        ssh_clean_pubkey_hash(&bufferSHA256);
        ssh_key_free(serverPublicKey);

        if(Exceptions::userAcceptsWarning("The authenticity of this host can't be established. Please "
                                          "verify the key fingerprints manually:\n\n" +
                                          hashes + "\nAre you sure you want to continue?")){

            // Add the host to the known hosts file
            ssh_session_update_known_hosts(ssh);

        } else {
            // User doesn't want to add host, fail quietly!
            ssh_free(ssh);
            throw new Error;
        }

        break;
    }

    case SSH_KNOWN_HOSTS_CHANGED:
    case SSH_KNOWN_HOSTS_OTHER:
    {
        ssh_free(ssh);
        throw new Error("WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!\nSomeone could be "
                        "eavesdropping on you right now (man-in-the-middle attack)! "
                        "It is also possible that a host key has just been changed.\n\n"
                        "Please use ssh to connect to the host and confirm the keys manually.");
        break;
    }


    // Uncovered error
    default:
        ssh_free(ssh);
        throw new Error("Key verification using your .ssh/known_hosts file failed. "
                        "Please use ssh to connect to the host and confirm the keys manually.");

    }


    // { Everything with the server is okay }

    if(sshSettings->useKeyfile()){
        // Try to load SSH key
        ssh_key privateKey;

        // First assume keyfile is unencrypted
        ok = ssh_pki_import_privkey_file(sshSettings->getKeyfile(), nullptr, nullptr, nullptr, &privateKey);

        // File cannot be accessed
        if(ok == SSH_EOF){
            throw new Error(QString("Keyfile '") + sshSettings->getKeyfile() + "' does not seem to exist or cannot be accessed.");
        }

        // File cannot be loaded
        if(ok == SSH_ERROR){
            // It didn't work, try asking for key decryption password
            PasswordDialog passwordDialog(nullptr, "Please enter the password to decrypt the keyfile:");
            passwordDialog.exec();
            const char *password = passwordDialog.getPassword();

            ok = ssh_pki_import_privkey_file(sshSettings->getKeyfile(), password, nullptr, nullptr, &privateKey);
        }


        // Neither loading plain nor loading encrypted keyfile worked
        if(ok != SSH_OK){
            throw new Error("Could not load given keyfile!");
        }


        // { Keyfile is ready for authentication }

        ok = ssh_userauth_publickey(ssh, sshSettings->getUsername(), privateKey);

        // Free key after authentication
        ssh_key_free(privateKey);

        if(ok != SSH_AUTH_SUCCESS){
            throw new Error("Authentication using the given keyfile failed!");
        }

    } else {
        // Don't use keyfiles, try password authentication

        QString userAndHost = QString(sshSettings->getUsername()) + "@" + QString(sshSettings->getHostname());
        PasswordDialog passwordDialog(nullptr, "Please enter the password to login as "+userAndHost+":");
        passwordDialog.exec();
        const char *password = passwordDialog.getPassword();

        ok = ssh_userauth_password(ssh, sshSettings->getUsername(), password);
    }


    // Check if the selected authentication method worked
    if(ok != SSH_AUTH_SUCCESS){
        qDebug() << "Error: " << QString(ssh_get_error(ssh));
        throw new Error("SSH Authentication on the remote host failed. Please try again!");
    }


    // { Authenticated connection is established }
    initSSHChannel();

    // Reader thread
    destroyAllThreads = false;
    sshCmd = "";

    readerThread = new std::thread([&]() {

        char buffer[8192];

        while(!destroyAllThreads){
            usleep(50000); // todo decrease this, investigate why it crashes < 100000

            sshMutex.lock();
            if(sshCmd != ""){
                char *data = sshCmd.toUtf8().data();
                ssh_channel_request_exec(sshChannel, data);
                //ssh_channel_write(sshChannel, data, strlen(data));
            }

            // Reset ssh stuff
            sshCmd = "";

            // If the channel isn't ready yet, wait for it!
            if(ssh_channel_is_eof(sshChannel) || !ssh_channel_is_open(sshChannel)){
                sshMutex.unlock();
                continue;
            }

            int bytesRead = ssh_channel_read_nonblocking(sshChannel, buffer, 8192, 0);
            sshMutex.unlock();

            if(bytesRead > 0){
                buffer[bytesRead] = '\0';
                QString dataString(buffer);

                emit remoteDataAvailable(dataString);
            }
        }
    });
}

Remote::~Remote()
{
    destroyAllThreads = true;
    readerThread->join();

    close();

    ssh_disconnect(ssh);
    ssh_free(ssh);
}


void Remote::initSSHChannel()
{
    sshChannel = ssh_channel_new(ssh);
    assert(sshChannel != nullptr);

    int ok;
    ok = ssh_channel_open_session(sshChannel);
    assert(ok == SSH_OK);
}


void Remote::run(QString cmd)
{
    sshMutex.lock();
    if(isRunning()){
        ssh_channel_close(sshChannel);
        ssh_channel_free(sshChannel);
    }

    initSSHChannel();

    sshCmd = cmd + "\n";
    sshMutex.unlock();
}

QString Remote::runAndWait(QString cmd)
{
    sshMutex.lock();

    // Issue normal 'run' to setup the channel, note the recursive locking!
    // This will prevent the other thread from actually executing the
    // command and reading our data.
    run(cmd);

    // So we have to run the command ourself
    char *data = sshCmd.toUtf8().data();
    ssh_channel_request_exec(sshChannel, data);

    // Block until the process ended
    ssh_channel_get_exit_status(sshChannel);
    int bytes = ssh_channel_poll(sshChannel, 0);

    // Error?
    if(bytes <= 0){
        sshMutex.unlock();
        return "";
    }

    // Copy received data
    char buffer[bytes];
    ssh_channel_read(sshChannel, (void *)buffer, (unsigned int)bytes, 0);
    QString resultString(buffer);

    // Reset channel for next commands
    sshCmd = "";
    sshMutex.unlock();

    return resultString;
}

void Remote::close()
{
    sshMutex.lock();

    sshCmd = "";
    ssh_channel_close(sshChannel);
    ssh_channel_free(sshChannel);

    sshMutex.unlock();
}


bool Remote::isRunning()
{
    return ssh_channel_is_open(sshChannel);
}


void Remote::processHasData()
{
    QString readString = "\n";
    readString = readString.left(readString.size()-1);

    emit remoteDataAvailable(readString);
}
