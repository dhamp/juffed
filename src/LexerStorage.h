/*
JuffEd - An advanced text editor
Copyright 2007-2009 Mikhail Murzin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _LEXER_STORAGE_H_
#define _LEXER_STORAGE_H_

class LSInterior;
class QsciLexer;

class QFont;
class QString;
class QStringList;

#include <QtGui/QColor>

class LexerStorage {
public:
	~LexerStorage();
	QString lexerName(const QString& fileName) const;
	QsciLexer* lexer(const QString& name, const QFont&);
	QColor curLineColor(const QString&) const;
	void updateLexer(const QString& name, const QFont& font);
	
	void getLexersList(QStringList&) const;

	static LexerStorage* instance();

protected:
	LexerStorage();
	
private:
	static LexerStorage* instance_;
	LSInterior* lsInt_;
};

#endif
