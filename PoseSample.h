/*
===============================================================================
File:       PoseSample.h
Author:     Clinton Freeman
Created:    May 16, 2011
===============================================================================
*/

#ifndef POSESAMPLE_H
#define POSESAMPLE_H

// Qt includes
#include <QtGui>

// system includes
#include <string>

class PoseSample
{
private:
    std::string name;
    QImage image;


public:
    inline std::string getName()
    {
        return name;
    }

    inline void setName(std::string& name)
    {
        this->name = name;
    }

    inline QImage getImage()
    {
        return image;
    }

    inline void setImage(QImage& image)
    {
        this->image = image;
    }
};

#endif // POSESAMPLE_H
