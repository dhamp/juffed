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

#include "MultiPage.h"

#include "Log.h"

//	Qt headers
#include <QtCore/QString>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QMap>

class MultiPageInterior {
public:
	MultiPageInterior(QWidget* mp) {
		tree_ = new QTreeWidget();
		tree_->setFixedWidth(180);
		tree_->header()->hide();
		tree_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//		tree_->setRootIsDecorated(false);
		
		panel_ = new QWidget();
		panelLayout_ = new QHBoxLayout();
		panelLayout_->setMargin(0);
		panel_->setLayout(panelLayout_);

		QHBoxLayout* hbox = new QHBoxLayout();
		hbox->setMargin(0);
		hbox->addWidget(tree_);
		hbox->addWidget(panel_);
		mp->setLayout(hbox);
	}
	~MultiPageInterior() {
		delete tree_;
		delete panel_;
	}

	QTreeWidget* tree_;
	QWidget* panel_;
	QHBoxLayout* panelLayout_;

	QMap<QTreeWidgetItem*, QWidget*> pages_;
};


MultiPage::MultiPage(QWidget* parent) : QWidget(parent) {
	JUFFENTRY2;
	mpInt_ = new MultiPageInterior(this);
	connect(mpInt_->tree_, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(changeCurrentItem(QTreeWidgetItem*, QTreeWidgetItem*)));
}

MultiPage::~MultiPage() {
	delete mpInt_;
}

void MultiPage::addPage(const QString& title, QWidget* w) {
	JUFFENTRY2;
	QTreeWidgetItem* it = new QTreeWidgetItem(QStringList(title));
	mpInt_->tree_->addTopLevelItem(it);
	w->setParent(mpInt_->panel_);
	mpInt_->pages_[it] = w;
	w->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	if ( pageCount() == 1 )
		selectPage(0);
	else
		selectPage(currentIndex());
}

void MultiPage::addChildPage(const QString& parentTitle, const QString& pageTitle, QWidget* w) {
	JUFFENTRY2;
	QList<QTreeWidgetItem*> items = mpInt_->tree_->findItems(parentTitle, Qt::MatchFixedString);
	if ( !items.isEmpty() ) {
		QTreeWidgetItem* p = items[0];

		QTreeWidgetItem* it = new QTreeWidgetItem(p, QStringList(pageTitle));
		w->setParent(mpInt_->panel_);
		mpInt_->pages_[it] = w;
		w->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		if ( pageCount() == 1 )
			selectPage(0);
		else
			selectPage(currentIndex());
		p->setExpanded(true);
	}
}

int MultiPage::pageCount() const {
	return mpInt_->pages_.count();
}

QWidget* MultiPage::currentPage() const {
	return mpInt_->pages_[mpInt_->tree_->currentItem()];
}

QWidget* MultiPage::page(const QString& title) const {
	QList<QTreeWidgetItem*> items = mpInt_->tree_->findItems(title, Qt::MatchFixedString);
	if ( items.count() > 0 ) {
		QTreeWidgetItem* item = items[0];
		if ( mpInt_->pages_.contains(item) ) {
			return mpInt_->pages_[item];
		}
	}
	return 0;
}

void MultiPage::selectPage(int page) {
	QTreeWidgetItem* it = mpInt_->tree_->topLevelItem(page);
	mpInt_->tree_->setCurrentItem(it);
	changeCurrentItem(it);
}

int MultiPage::currentIndex() const {
	QTreeWidgetItem* it = mpInt_->tree_->currentItem();
	return mpInt_->tree_->indexOfTopLevelItem(it);
}

void MultiPage::changeCurrentItem(QTreeWidgetItem* it, QTreeWidgetItem*) {
	if ( it ) {
		QWidget* page = mpInt_->pages_[it];
		if ( page )  {
			foreach (QWidget* w, mpInt_->pages_.values()) {
				w->hide();
			}
			while ( mpInt_->panelLayout_->count() > 0 ) {
				mpInt_->panelLayout_->removeItem(mpInt_->panelLayout_->itemAt(0));
			}
			mpInt_->panelLayout_->addWidget(page);
			page->show();
		}
	}
}

QStringList MultiPage::getChildrenTitles(const QString& parentTitle) {
	QStringList list;
	QList<QTreeWidgetItem*> items = mpInt_->tree_->findItems(parentTitle, Qt::MatchFixedString);
	if ( items.count() > 0 ) {
		QTreeWidgetItem* pluginsItem = items[0];
		int n = pluginsItem->childCount();
		for (int i = 0; i < n; ++i ) {
			QTreeWidgetItem* it = pluginsItem->child(i);
			if ( it ) {
				list << it->text(0);
			}
		}
	}
	return list;
}
