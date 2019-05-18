#include "remote.h"
#include "passworddialog.h"

#include <QProcess>
#include <QDebug>

Remote::Remote(QObject *qObject, QString hostnameString, QString usernameString)
{
	ssh = ssh_new();
	assert(ssh != nullptr);

	ssh_options_set(ssh, SSH_OPTIONS_HOST, "localhost");
	int ok;

	ok = ssh_connect(ssh);

	if(ok != SSH_OK){
		// todo: catch this
		ssh_free(ssh);
		assert(ok == SSH_OK);
	}

	unsigned char *hash = nullptr;
	ssh_key srv_pubkey = nullptr;
	size_t hlen;

	ok = ssh_get_server_publickey(ssh, &srv_pubkey);
	assert(ok>=0);

	ok = ssh_get_publickey_hash(srv_pubkey,
								SSH_PUBLICKEY_HASH_SHA1,
								&hash,
								&hlen);
	ssh_key_free(srv_pubkey);
	assert(ok >= 0);

	ok = ssh_session_is_known_server(ssh);
	assert(ok >= 0);

	qDebug() << "connection established!";


	// Get the required password
	QString passwordString;
	PasswordDialog passwordDialog(nullptr, &passwordString);
	passwordDialog.exec();

	const char *password = passwordString.toUtf8().data();
	const char *username = usernameString.toUtf8().data();
	const char *hostname = hostnameString.toUtf8().data();

	ok = ssh_userauth_password(ssh, username, password);
	assert(ok == SSH_AUTH_SUCCESS);

	qDebug() << "Authenticated :)";


}


Remote::~Remote()
{
	ssh_disconnect(ssh);
	ssh_free(ssh);
}


void Remote::run(QString cmd)
{

}

void Remote::close()
{

}


bool Remote::isRunning()
{
	return false;
}


void Remote::processHasData()
{
	QString readString = "\n";
	readString = readString.left(readString.size()-1);

	emit remoteDataAvailable(readString);
}
