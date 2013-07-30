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

#ifndef _KAR_HPP_
#define _KAR_HPP_

#include <QString>
#include <QMap>
#include <QStringList>
#include <QByteArray>
#include <QDataStream>
#include <QSharedPointer>

#include "export.h"

#define KAR_SPECIAL_PREFIX "internal::"

namespace kiss
{
	class Kar;
	
	typedef QSharedPointer<Kar> KarPtr;
	
	class DLL_EXPORT Kar
	{
	public:
		typedef QMap<QString, QByteArray> DataMap;
		
		Kar();
		Kar(const DataMap& data);
		
		bool addFile(const QString& name, const QByteArray& data);
		void setFile(const QString& name, const QByteArray& data);
		bool removeFile(const QString& name);
		bool hasFile(const QString& name);
		bool rename(const QString& name, const QString& newName);
		
		QByteArray data(const QString& name) const;
		QStringList files() const;
		
		static KarPtr create();
		static KarPtr create(const QString& path);
		static KarPtr load(const QString& path);
		
		bool save(const QString& path);
		bool extract(const QString& path) const;
		
		const DataMap& data() const;
		void setData(const DataMap& data);
		
		static bool isValid(const QString &path);
		
	private:
		static void create(const KarPtr& archive, const QString& path, const QString& subpath);
		bool extract(const QString& path, const QString& subpath) const;
		
		DataMap m_data;
	};
}

DLL_EXPORT QDataStream& operator<<(QDataStream& out, const kiss::Kar& kar);
DLL_EXPORT QDataStream& operator>>(QDataStream& in, kiss::Kar& kar);

#endif

