/*****************************************************************************
 * MeshLab                                                           o o     *
 * A versatile mesh processing toolbox                             o     o   *
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

#ifndef MESHLAB_FLOAT_VALUE_H
#define MESHLAB_FLOAT_VALUE_H

#include "value.h"

class FloatValue : public Value
{
public:
	FloatValue(const float val);
	~FloatValue() {}

	Scalarm     getFloat() const;
	bool        isFloat() const;
	QString     typeName() const;
	void        set(const Value& p);
	FloatValue* clone() const;
	bool        operator==(const Value& p) const;
	void        fillToXMLElement(QDomElement& element) const;

private:
	Scalarm pval;
};

#endif // MESHLAB_FLOAT_VALUE_H
