#include "FlapInd.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "TFT_eSPI.h"
#include "Images\background.h"
#include "Images\handle.h"

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite backgroundSprite = TFT_eSprite(&tft);
TFT_eSprite handleSprite = TFT_eSprite(&backgroundSprite);

FlapInd::FlapInd()
{
}

void FlapInd::begin()
{
    tft.init();
    tft.setRotation(0);

    backgroundSprite.createSprite(tft.width(), tft.height());

    handleSprite.createSprite(HANDLE_IMG_WIDTH, HANDLE_IMG_HEIGHT);
    handleSprite.pushImage(0, 0, HANDLE_IMG_WIDTH, HANDLE_IMG_HEIGHT, HANDLE_IMG_DATA);
    
    draw();
}

void FlapInd::attach()
{
}

void FlapInd::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void FlapInd::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    int32_t  data = atoi(setPoint);
    uint16_t output;

    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
    case -2:
        // tbd., get's called when PowerSavingMode is entered
    case 0:
        _flapIndicatorPct = atof(setPoint);
        draw();
        break;
    case 1:
        _flapHandlePct = atof(setPoint);
        draw();
        break;
    default:
        break;
    }
}

void FlapInd::draw()
{
    backgroundSprite.fillSprite(TFT_BLACK);
    backgroundSprite.pushImage(0, 0, BACKGROUND_IMG_WIDTH, BACKGROUND_IMG_HEIGHT, BACKGROUND_IMG_DATA);

//    tft.fillRect(BACKGROUND_IMG_WIDTH, 0, 40, tft.height(), TFT_BLACK);
    backgroundSprite.fillCircle(BACKGROUND_IMG_WIDTH + 20, (_flapIndicatorPct * (tft.height() - 40))  + 20, 15, TFT_RED);

    handleSprite.pushToSprite(&backgroundSprite, 20, (_flapHandlePct * (tft.height() - 40)), TFT_WHITE);  

    backgroundSprite.pushSprite(0,0);
}



void FlapInd::update()
{
    // Do something which is required regulary
}
