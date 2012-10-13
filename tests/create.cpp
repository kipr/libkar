#include "kar.hpp"

using namespace Kiss;

#include <QDebug>

int main(int argc, char *argv[])
{
	KarPtr archive = Kar::create("/Users/beta/Desktop/extract_test");
	QStringList files = archive->files();
	qDebug() << files;
	if(files.isEmpty()) return 0;
	
	foreach(const QString& file, files)
		qDebug() << file << "=" << archive->data(file);
	return 0;
}