/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "kar.hpp"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>

using namespace Kiss;

#define MAX_FILENAME_SIZE 128

#define KAR_MAGIC ("karkar")

bool Kar::addFile(const QString& name, const QByteArray& data)
{
	if(m_data.contains(name)) return false;
	m_data.insert(name, data);
	return true;
}

void Kar::setFile(const QString& name, const QByteArray& data)
{
	m_data.remove(name);
	m_data.insert(name, data);
}

bool Kar::removeFile(const QString& name)
{
	if(!m_data.contains(name)) return false;
	m_data.remove(name);
	return true;
}

bool Kar::hasFile(const QString& name)
{
	return m_data.contains(name);
}

bool Kar::rename(const QString& name, const QString& newName)
{
	if(!m_data.contains(name) || m_data.contains(newName)) return false;
	
	m_data[newName] = m_data[name];
	m_data.remove(name);
	
	return true;
}

QByteArray Kar::data(const QString& name) const
{
	return m_data.value(name, QByteArray());
}

QStringList Kar::files() const
{
	return QStringList(m_data.keys());
}

KarPtr Kar::create()
{
	return KarPtr(new Kar());
}

KarPtr Kar::create(const QString& path)
{
	KarPtr archive = create();
	create(archive, path, "");
	return archive;
}

KarPtr Kar::load(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return KarPtr();
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_0);
	QMap<QString, QByteArray> compressedData;
	char *magic;
	stream >> magic;
	const bool good = strcmp(magic, KAR_MAGIC) != 0;
	delete[] magic;
	if(!good) {
		file.close();
		return KarPtr();
	}
	stream >> compressedData;
	QMap<QString, QByteArray> data;
	foreach(const QString& key, compressedData.keys()) {
		data.insert(key, qUncompress(compressedData[key]));
	}
	file.close();
	return KarPtr(new Kar(data));
}

bool Kar::save(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::WriteOnly)) return false;
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_0);
	QMap<QString, QByteArray> compressedData;
	foreach(const QString& key, m_data.keys()) {
		compressedData.insert(key, qCompress(m_data[key]));
	}
	stream << KAR_MAGIC;
	stream << compressedData;
	file.close();
	return true;
}

bool Kar::extract(const QString& path) const
{
	return extract(path, "");
}

const Kar::DataMap& Kar::data() const
{
	return m_data;
}

void Kar::setData(const Kar::DataMap& data)
{
	m_data = data;
}

Kar::Kar()
{
}

Kar::Kar(const Kar::DataMap& data)
	: m_data(data)
{
}

void Kar::create(const KarPtr& archive, const QString& path, const QString& subpath)
{
	QFileInfoList files = QDir(path + (subpath.isEmpty() ? "" : "/" + subpath)).entryInfoList(QDir::Dirs
		| QDir::Files | QDir::NoDot | QDir::NoDotDot);
	foreach(const QFileInfo& file, files) {
		if(file.isDir()) {
			create(archive, path, (subpath.isEmpty() ? "" : subpath + "/") + file.fileName());
			continue;
		}
		QFile f(file.absoluteFilePath());
		if(!f.open(QIODevice::ReadOnly)) continue;
		archive->addFile((subpath.isEmpty() ? "" : subpath + "/") + file.fileName(), f.readAll());
		f.close();
	}
}

bool Kar::extract(const QString& path, const QString& subpath) const
{
	if(!QDir().mkpath(path + "/" + subpath)) {
		qCritical() << "Failed to create path" << path + "/" + subpath;
		return false;
	}
	
	QStringList pathEntries;
	foreach(const QString& file, files()) {
		if(!subpath.isEmpty() && !file.startsWith(subpath + "/")) continue;
		pathEntries << file.mid(subpath.isEmpty() ? 0 : subpath.size() + 1);
	}
	
	QStringList pathFiles;
	QStringList pathDirs;
	foreach(const QString& entry, pathEntries) {
		if(entry.contains("/")) {
			QString name = entry.section("/", 0, 0);
			if(!name.isEmpty()) pathDirs << name;
			continue;
		}
		pathFiles << entry;
	}
	
	pathDirs.removeDuplicates();
	
	foreach(const QString& dir, pathDirs) {
		const QString dirPath = (subpath.isEmpty() ? "" : subpath + "/") + dir;
		if(!extract(path, dirPath)) return false;
	}
	
	foreach(const QString& file, pathFiles) {
		const QString filePath = (subpath.isEmpty() ? "" : subpath + "/") + file;
		QFile f(path + "/" + filePath);
		if(!f.open(QIODevice::WriteOnly)) {
			qWarning() << "Failed to write" << path + "/" + filePath;
			continue;
		}
		f.write(data(filePath));
		f.close();
	}
	return true;
}

QDataStream& operator<<(QDataStream& out, const Kiss::Kar& kar)
{
	QMap<QString, QByteArray> compressedData;
	foreach(const QString& key, kar.data().keys()) {
		compressedData.insert(key, qCompress(kar.data()[key]));
	}
	out << KAR_MAGIC;
	out << compressedData;
	return out;
}

QDataStream& operator>>(QDataStream& in, Kiss::Kar& kar)
{
	QMap<QString, QByteArray> compressedData;
	char *magic;
	in >> magic;
	const bool good = strcmp(magic, KAR_MAGIC) != 0;
	delete[] magic;
	if(!good) return in;
	in >> compressedData;
	Kar::DataMap data;
	foreach(const QString& key, compressedData.keys()) {
		data.insert(key, qUncompress(compressedData[key]));
	}
	kar.setData(data);
	return in;
}