#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Path2d.h"
#include "cinder/Display.h"
#include "cinder/GeomIo.h"
#include "cinder/app/Window.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "stack.h"
#include "calculatorClass.h"
#include <string>

using namespace ci;
using namespace ci::app;
using namespace std;

const int yCutoff = 80, spacing = 130, xwinMax = 3.5 * spacing, ywinMax = (2.5 * spacing) + yCutoff + 25, buttonHeight = 65, buttonLength = 115;

class Honors_ProjectApp : public App

{
private:
    DisplayRef display = Display::getMainDisplay(); //Pointer to main display (Needed to make everything fit and look nice and presentable)
    calculator calcapp;
    Rectf resizedArea, areatoFill;
    Font outputFont = Font("Times New Roman", yCutoff);
    vec2 outputBoxSize;
    string infixexpression;
    bool shift, answerRequested;
    ivec2 mouseLoc;
    
public:
    gl::Texture2dRef buttonsimg;
    gl::Texture2dRef output;
    TextBox resultBox;
    
    void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown( KeyEvent event) override;
	void update() override;
	void draw() override;
    void render();
};

void Honors_ProjectApp::setup() //Get all and initialize all resources needed for the calculator
{
    resizedArea = Rectf(0, yCutoff, 500, 600);
    areatoFill = Rectf(0, yCutoff, xwinMax, ywinMax);
    setWindowSize(xwinMax, ywinMax);
    auto img = loadImage(loadAsset("applebuttons.jpg"));
    buttonsimg = gl::Texture2d::create(img);
    
    outputBoxSize = vec2( xwinMax, yCutoff );
    render();
    
    shift = answerRequested = false;
}

void Honors_ProjectApp::render()
{
    resultBox = TextBox().alignment( TextBox::RIGHT ).font( outputFont ).size( ivec2( outputBoxSize ) ).text( calcapp.getInitial() );
    
    resultBox.setColor( Color( 0.89f, 0.47f, 0.20f ) );
    resultBox.setBackgroundColor( Color( .8, .8, .8 ) );
    output = gl::Texture2d::create(resultBox.render()); //Renders initial state of resultBox right when the calculator is open, then creates it into something gl::draw() is able to draw
}

void Honors_ProjectApp::mouseDown( MouseEvent event )
{
    mouseLoc = event.getPos();
    
    if(event.isLeft())
    {
        if(mouseLoc.y >= ywinMax - buttonHeight && mouseLoc.y <= ywinMax)
        {
            if(mouseLoc.x >= 0 && mouseLoc.x <= buttonLength)
            {
                infixexpression += '0';
            }
            else if(mouseLoc.x >= buttonLength + 1 && mouseLoc.x <= 2 * buttonLength)
            {
                infixexpression.erase(0, infixexpression.size());
            }
            else if(mouseLoc.x >= (2 * buttonLength) + 1 && mouseLoc.x <= 3 * buttonLength)
            {
                infixexpression += 'p';
            }
            else
            {
                if(calcapp.checkString(infixexpression))
                {
                    infixexpression = to_string(calcapp.calc((infixexpression)));
                }
                else
                {
                    infixexpression = "ERROR";
                }
                
                answerRequested = true;
            }
        }
        else if(mouseLoc.y >= ywinMax - (buttonHeight * 2) - 5 && mouseLoc.y <= ywinMax - buttonHeight - 1)
        {
            if(mouseLoc.x >= 0 && mouseLoc.x <= buttonLength)
            {
                infixexpression += '1';
            }
            else if(mouseLoc.x >= buttonLength + 1 && mouseLoc.x <= 2 * buttonLength)
            {
                infixexpression += '2';
            }
            else if(mouseLoc.x >= (2 * buttonLength) + 1 && mouseLoc.x <= 3 * buttonLength)
            {
                infixexpression += '3';
            }
            else
            {
                infixexpression += '+';
            }
        }
        else if(mouseLoc.y >= ywinMax - (buttonHeight * 3) - 15 && mouseLoc.y <= ywinMax - (buttonHeight * 2) - 1)
        {
            if(mouseLoc.x >= 0 && mouseLoc.x <= buttonLength)
            {
                infixexpression += '4';
            }
            else if(mouseLoc.x >= buttonLength + 1 && mouseLoc.x <= 2 * buttonLength)
            {
                infixexpression += '5';
            }
            else if(mouseLoc.x >= (2 * buttonLength) + 1 && mouseLoc.x <= 3 * buttonLength)
            {
                infixexpression += '6';
            }
            else
            {
                infixexpression += '-';
            }
        }
        else if(mouseLoc.y >= ywinMax - (buttonHeight * 4) - 18 && mouseLoc.y <= ywinMax - (buttonHeight * 3) - 1)
        {
            if(mouseLoc.x >= 0 && mouseLoc.x <= buttonLength)
            {
                infixexpression += '7';
            }
            else if(mouseLoc.x >= buttonLength + 1 && mouseLoc.x <= 2 * buttonLength)
            {
                infixexpression += '8';
            }
            else if(mouseLoc.x >= (2 * buttonLength) + 1 && mouseLoc.x <= 3 * buttonLength)
            {
                infixexpression += '9';
            }
            else
            {
                infixexpression += '*';
            }
        }
        else if(mouseLoc.y >= yCutoff && mouseLoc.y <= ywinMax - (buttonHeight * 4) - 1)
        {
            if(mouseLoc.x >= 0 && mouseLoc.x <= buttonLength)
            {
                infixexpression += '(';
            }
            else if(mouseLoc.x >= buttonLength + 1 && mouseLoc.x <= 2 * buttonLength)
            {
                infixexpression += ')';
            }
            else if(mouseLoc.x >= (2 * buttonLength) + 1 && mouseLoc.x <= 3 * buttonLength)
            {
                infixexpression += 's';
            }
            else
            {
                infixexpression += '/';
            }
        }
    }
}

void Honors_ProjectApp::keyDown( KeyEvent event)
{
    if(event.getCode() == KeyEvent::KEY_SPACE) //Reset to default window size
    {
        setWindowSize(xwinMax, ywinMax);
    }
    else if(event.isShiftDown()) //Checks if shift was pressed (For laptops specifically where it requires you to hit shift to get addition)
    {
        shift = true;
    }
    else if(shift) //If shift was pressed, reset shift to unpressed state, then decide what the shifted value of the key is and add it to infix expression
    {
        shift = false;
        if(event.getChar() == '=')
        {
            infixexpression += '+';
        }
        else if(event.getChar() == '9')
        {
            infixexpression += '(';
        }
        else if(event.getChar() == '0')
        {
            infixexpression += ')';
        }
        else if(event.getChar() == '8')
        {
            infixexpression += '*';
        }
    }
    else if(event.getCode() == KeyEvent::KEY_BACKSPACE)
    {
        if(!answerRequested)
        {
            infixexpression.erase(infixexpression.size() - 1, 1);
        }
        else
        {
            infixexpression.erase(0, infixexpression.size());
            answerRequested = false;
        }
    }
    else if(event.getCode() == KeyEvent::KEY_KP_ENTER || event.getCode() == KeyEvent::KEY_RETURN) //If enter or return is hit, check if the infixexpression is valid, if not output an error, if valid, calculate result
    {
        if(calcapp.checkString(infixexpression))
        {
            infixexpression = to_string(calcapp.calc((infixexpression)));
        }
        else
        {
            infixexpression = "ERROR";
        }
        
        answerRequested = true;
    }
    else
    {
        infixexpression += event.getChar();
    }
    
}

void Honors_ProjectApp::update()
{
    resultBox.setText(infixexpression); //Render the resultBox, then create it into something we can draw with gl::draw()
    output = gl::Texture2d::create(resultBox.render());
}

void Honors_ProjectApp::draw()
{
    gl::setMatricesWindow( getWindowSize() );
    gl::enableAlphaBlending();
	gl::clear( Color( .8, .8, .8 ) );
    
    
    gl::draw(buttonsimg, Area(resizedArea.getCenteredFit(buttonsimg->getBounds(), true)), areatoFill); //Draw the calculator pic
    gl::draw(output); //Draw resultBox
    
}

CINDER_APP( Honors_ProjectApp, RendererGl )