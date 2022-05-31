/*****************************************************************************
 * MeshLab                                                           o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2004-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 *                                                                           *
 ****************************************************************************/

#include "direction_widget.h"

#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QFileDialog>
#include <common/ml_document/mesh_document.h>

DirectionWidget::DirectionWidget(
	QWidget*             p,
	const RichDirection& rpf,
	const RichDirection& rdef,
	QWidget*             gla_curr) :
		RichParameterWidget(p, rpf, rdef)
{
	paramName = rpf.name();

	vlay = new QHBoxLayout();
	vlay->setSpacing(0);
	for (int i = 0; i < 3; ++i) {
		coordSB[i]     = new QLineEdit(this);
		QFont baseFont = coordSB[i]->font();
		if (baseFont.pixelSize() != -1)
			baseFont.setPixelSize(baseFont.pixelSize() * 3 / 4);
		else
			baseFont.setPointSize(baseFont.pointSize() * 3 / 4);
		coordSB[i]->setFont(baseFont);
		coordSB[i]->setMinimumWidth(coordSB[i]->sizeHint().width() / 4);
		coordSB[i]->setValidator(new QDoubleValidator());
		coordSB[i]->setAlignment(Qt::AlignRight);
		coordSB[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
		vlay->addWidget(coordSB[i]);
		widgets.push_back(coordSB[i]);
		connect(coordSB[i], SIGNAL(textChanged(QString)), this, SLOT(setParameterChanged()));
	}
	this->setValue(paramName, parameter->value().getPoint3());
	// if we have a connection to the current glarea we can setup the additional
	// button for getting the current view direction.
	if (gla_curr) {
		QStringList names;
		names << "View Dir.";
		names << "Raster Camera Dir.";

		getPoint3Combo = new QComboBox(this);
		getPoint3Combo->addItems(names);
		getPoint3Combo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		vlay->addWidget(getPoint3Combo);
		widgets.push_back(getPoint3Combo);

		connect(
			gla_curr,
			SIGNAL(transmitViewDir(QString, Point3m)),
			this,
			SLOT(setValue(QString, Point3m)));
		connect(
			gla_curr,
			SIGNAL(transmitShot(QString, Shotm)),
			this,
			SLOT(setShotValue(QString, Shotm)));
		connect(this, SIGNAL(askViewDir(QString)), gla_curr, SLOT(sendViewDir(QString)));
		connect(this, SIGNAL(askCameraDir(QString)), gla_curr, SLOT(sendRasterShot(QString)));

		getPoint3Button = new QPushButton("Get", this);
		getPoint3Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		connect(getPoint3Button, SIGNAL(clicked()), this, SLOT(getPoint()));

		vlay->addWidget(getPoint3Button);
		widgets.push_back(getPoint3Button);
	}
}

DirectionWidget::~DirectionWidget()
{
	this->disconnect();
}

void DirectionWidget::addWidgetToGridLayout(QGridLayout* lay, const int r)
{
	if (lay != nullptr) {
		lay->addLayout(vlay, r, 1);
	}
	RichParameterWidget::addWidgetToGridLayout(lay, r);
}

std::shared_ptr<Value> DirectionWidget::getWidgetValue() const
{
	return std::make_shared<Point3Value>(Point3m(
		coordSB[0]->text().toFloat(), coordSB[1]->text().toFloat(), coordSB[2]->text().toFloat()));
}

void DirectionWidget::collectWidgetValue()
{
	parameter->setValue(Point3Value(vcg::Point3f(
		coordSB[0]->text().toFloat(), coordSB[1]->text().toFloat(), coordSB[2]->text().toFloat())));
}

void DirectionWidget::resetWidgetValue()
{
	for (unsigned int ii = 0; ii < 3; ++ii)
		coordSB[ii]->setText(QString::number(parameter->value().getPoint3()[ii], 'g', 3));
}

void DirectionWidget::setWidgetValue(const Value& nv)
{
	for (unsigned int ii = 0; ii < 3; ++ii)
		coordSB[ii]->setText(QString::number(nv.getPoint3()[ii], 'g', 3));
}

vcg::Point3f DirectionWidget::getValue()
{
	return vcg::Point3f(
		coordSB[0]->text().toFloat(), coordSB[1]->text().toFloat(), coordSB[2]->text().toFloat());
}

void DirectionWidget::getPoint()
{
	int index = getPoint3Combo->currentIndex();
	switch (index) {
	case 0: emit askViewDir(paramName); break;
	case 1: emit askCameraDir(paramName); break;

	default: assert(0);
	}
}

void DirectionWidget::setValue(QString name, Point3m newVal)
{
	if (name == paramName) {
		for (int i = 0; i < 3; ++i)
			coordSB[i]->setText(QString::number(newVal[i], 'g', 4));
	}
}

void DirectionWidget::setShotValue(QString name, Shotm newValShot)
{
	vcg::Point3f p = newValShot.GetViewDir();
	setValue(name, p);
}