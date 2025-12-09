#pragma once

#ifndef VIEWBOX_H_
#define VIEWBOX_H_

#include <string>
using namespace std;

class viewbox {
private:
    float viewX, viewY, viewWidth, viewHeight;
    float portWidth, portHeight;
    string preservedForm, preservedMode;
    float rotationAngle;

public:
  
    viewbox();

  
    float getViewX() const;
    float getViewY() const;
    float getViewWidth() const;
    float getViewHeight() const;
    float getPortWidth() const;
    float getPortHeight() const;
    float getRotationAngle() const;

 
    void setViewX(float);
    void setViewY(float);
    void setViewWidth(float);
    void setViewHeight(float);
    void setPortWidth(float);
    void setPortHeight(float);
    void setRotationAngle(float);

    
};


#endif