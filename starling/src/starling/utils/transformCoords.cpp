// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "transformCoords.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"

    /** Uses a matrix to transform 2D coordinates into a different space. If you pass a 
     *  'resultPoint', the result will be stored in this point instead of creating a new object.*/
    Point* ::transformCoords(Matrix* matrix, float x, float y,
                                    Point* resultPoint)
    {
        if (!deprecationNotified)
        {
            deprecationNotified = true;
            trace("[Starling] The method 'transformCoords' is deprecated. " +
                  "Please use 'MatrixUtil.transformCoords' instead.");
        }

        if (resultPoint == NULL) resultPoint = new Point();

        resultPoint->x ( matrix->a * x + matrix->c * y + matrix->tx());
        resultPoint->y ( matrix->d * y + matrix->b * x + matrix->ty());

        return resultPoint;
    }


 bool deprecationNotified;
