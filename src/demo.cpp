#include "QtCore/QtCore"
#include "qhttpservice.h"

void basic_auth(QHttpRequest & req, QHttpRespone & res, QHttpContext & ctx)
{
	bool approve = false;

	QString a = req.headers["Authorization"];
	if (a.startWith("Basic ")) 
	{
		QStringList x = QByteArray::fromBase64(a.mid(6).trim()).toString().split(":");
		if (x[0] == "jcz" && x[1] == "123456") {
			approve = true;
			req["user"] = "jcz";
			ctx.next();
		}
	}
	if (!approve)
   	{
		res.headers["WWW-Authenticate"] = "Basic realm=\"private area\"";
		res.status(401);
		res.end();
		return;
	}
}

void hello_world(QHttpRequest & req, QHttpRespone & res, QHttpContext & ctx)
{
	res.send("Hello " + req["user"]);
}

int main(int argc, char * argv[])
{
	QHttpApplication app(argc, argv);

	app.get("/", hello_world);

	return app.exec();
}
