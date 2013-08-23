#ifndef OFXTEXTUREDUI_H
#define OFXTEXTUREDUI_H

#include "ofMain.h"
#include "ofxUIWidgetWithLabel.h"

class ofxTexturedUI : public ofxUIWidgetWithLabel
{
public:
    ofxTexturedUI(float x, float y, float w, float h, ofTexture *_texture, string _name) : ofxUIWidgetWithLabel()
    {
        rect = new ofxUIRectangle(x,y,w,h); 
        init(w, h, _texture, _name); 
    }
    
    ofxTexturedUI(float x, float y, float w, float h, ofTexture *_texture, string _name, bool _showLabel) : ofxUIWidgetWithLabel()
    {
        rect = new ofxUIRectangle(x,y,w,h); 
        init(w, h, _texture, _name); 
        showLabel = _showLabel; 
    }
    
    ofxTexturedUI(float w, float h, ofTexture *_texture, string _name) : ofxUIWidgetWithLabel()
    {
        rect = new ofxUIRectangle(0,0,w,h); 
        init(w, h, _texture, _name); 
    }    

    ofxTexturedUI(float w, float h, ofTexture *_texture, string _name, bool _showLabel) : ofxUIWidgetWithLabel()
    {
        rect = new ofxUIRectangle(0,0,w,h); 
        init(w, h, _texture, _name); 
        showLabel = _showLabel; 
    }    
    
    void init(float w, float h, ofTexture *_texture, string _name)
    {
		name = string(_name);  				
		kind = OFX_UI_WIDGET_IMAGE; 
        showLabel = true; 
		paddedRect = new ofxUIRectangle(-padding, -padding, w+padding*2.0, h+padding);
		paddedRect->setParent(rect); 
        
        draw_back = false; 
        draw_fill = true; 
        
		texture = _texture;
		//image = _image; 

        
		label = new ofxUILabel(0,h+padding,(name+" LABEL"),name, OFX_UI_FONT_SMALL); 		
		label->setParent(label); 
		label->setRectParent(rect);    
        label->setEmbedded(true);
        cropImageToFitRect = false; 
    }
    
    virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
        label->setDrawPadding(false);
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
        label->setDrawPaddingOutline(false);
	}  
    
    virtual void drawFill()
    {
        if(draw_fill)
        {			
			if(texture != NULL)
			{			   
				ofFill(); 
				ofSetColor(255);
                if(cropImageToFitRect)
                {	
					std::cout<<"x = "<<rect->getX()<<" y= "<< rect->getY()<<" w= "<<rect->width<<" h="<<rect->height<<std::endl;
					texture->drawSubsection(rect->getX(), rect->getY(), rect->width, rect->height, 0, 0, rect->width, rect->height);
                    //image->drawSubsection(rect->getX(), rect->getY(), rect->width, rect->height, 0, 0, rect->width, rect->height);
                }
                else
                {
                    //image->draw(rect->getX(), rect->getY(), rect->width, rect->height);
					
					texture->draw(rect->getX(), rect->getY(), rect->width, rect->height);
                }
			}
        }
    }        

    virtual void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(showLabel);             
    }
		
    void setCropImageToFitRect(bool _cropImageToFitRect)
    {
        cropImageToFitRect = _cropImageToFitRect;
    }
    
	ofxUILabel *getLabel()
	{
		return label; 
	}
    
    //void setImage(ofImage *_image)
    //{
    //    image = _image; 
    //}

    void setTexture(ofTexture*_texture)
    {
        texture = _texture; 
    }

	
	virtual void setParent(ofxUIWidget *_parent)
	{
		parent = _parent; 
        if(showLabel)
        {
            paddedRect->height += label->getPaddingRect()->height; 		
        }    
        else
        {
            paddedRect->height += padding;
        }
        label->setVisible(showLabel);             
	}	

    virtual bool isDraggable()
    {
        return false; 
    }
    
    
protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent; 
	ofTexture *texture;
    bool showLabel;
    bool cropImageToFitRect; 
}; 

#endif