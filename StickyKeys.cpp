#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
using namespace std;

//Create the actual Game Engine
class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "Sticky Keys Engine";
    }

private:
    string LevelOne;
    int nLevelWidth;
    int nLevelHeight;

    int coinCounter = 0;
    int totalcoins = 0;

    float fPlayerX = 0.0f;
    float fPlayerY = 0.0f;

    float playerVelX = 0.0f;
    float playerVelY = 0.0f;

    float mouseX = GetMouseX();
    float mouseY = GetMouseY();

    float cameraX = 0.0f;
    float cameraY = 0.0f;

    // Sprite Resources
    olc::Sprite *spriteTiles = nullptr;
    olc::Sprite *spriteMan = nullptr;
    olc::Sprite *spriteCoin = nullptr;

    // Sprite selection flags
    int nDirModX = 0;
    int nDirModY = 0;

    bool onGround = false;
    bool canMoveLeft = true;
    bool canMoveRight = true;
    bool canJump = true;

    bool leftClicked = false;
    bool leftPlaced = false;
    bool rightClicked = false;
    bool rightPlaced = false;
    bool jumpClicked = false;
    bool jumpPlaced = false;

    bool fixTiles = false;

public:
    //Create initial level and load any Sprites needed at this stage
    bool OnUserCreate() override
    {
        nLevelWidth = 64;
        nLevelHeight = 16;

        LevelOne += "................................................................";
        LevelOne += "................................................................";
        LevelOne += "................................................................";
        LevelOne += ".......#####....................................................";
        LevelOne += "........###..............ooo....................................";
        LevelOne += ".......................########.................................";
        LevelOne += ".....##########.......###..............#.#......................";
        LevelOne += "....................###................#.#......................";
        LevelOne += "...................####.........................................";
        LevelOne += "####################################.##############.....########";
        LevelOne += "...................................#.#...............###........";
        LevelOne += "........................############.#............###...........";
        LevelOne += "........................#............#.........###..............";
        LevelOne += "........................#.############.....####.................";
        LevelOne += "........................#................##.....................";
        LevelOne += "........................#################.......................";

        spriteMan = new olc::Sprite("/home/jtorreal/CLionProjects/StickyKeys2.0/Images/MarioSkidding.png");
        spriteTiles = new olc::Sprite("/home/jtorreal/CLionProjects/StickyKeys2.0/Images/tut_tiles.png");
        spriteCoin = new olc::Sprite("/home/jtorreal/CLionProjects/StickyKeys2.0/Images/coin.png");
        for (int i; i<LevelOne.length();i++){
            if(LevelOne[i]=='o') {
                totalcoins = totalcoins + 1;
            }
        }
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        auto GetTile = [&](int x, int y)
        {
            if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
                return LevelOne[y * nLevelWidth + x];
            else
                return ' ';
        };

        auto SetTile = [&](int x, int y, char c)
        {
            if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
                LevelOne[y * nLevelWidth + x] = c;
        };

        SetPixelMode(olc::Pixel::ALPHA);
        //No longer needed as they are updated between frames
        //playerVelY = 0.0f;
        //playerVelX = 0.0f;

        //Handle Input
        if(IsFocused()){
            if(GetKey(olc::Key::UP).bPressed && canJump){
                if(playerVelY == 0)
                    playerVelY = -12.0f;
            }
            if(GetKey(olc::Key::LEFT).bHeld && canMoveLeft){
                playerVelX = -6.0f;
            }
            if(GetKey(olc::Key::A).bHeld && canMoveLeft){
                playerVelX = -6.0f;
            }
            if(GetKey(olc::Key::LEFT).bReleased){
                playerVelX = 0.0f;
            }
            if(GetKey(olc::Key::A).bReleased){
                playerVelX = 0.0f;
            }
            if(GetKey(olc::Key::RIGHT).bHeld && canMoveRight) {
                playerVelX = 6.0f;
            }
            if(GetKey(olc::Key::D).bHeld && canMoveRight) {
                playerVelX = 6.0f;
            }
            if(GetKey(olc::Key::RIGHT).bReleased){
                playerVelX = 0.0f;
            }
            if(GetKey(olc::Key::D).bReleased){
                playerVelX = 0.0f;
            }
            if(GetKey(olc::Key::SPACE).bPressed && canJump) {
                if(playerVelY == 0){
                    playerVelY = -12.0f;
                }
            }
            if(GetKey(olc::Key::W).bPressed && canJump) {
                if(playerVelY == 0){
                    playerVelY = -12.0f;
                }
            }
            if(GetKey(olc::Key::R).bPressed) {
                canJump = true;
                canMoveRight = true;
                canMoveLeft = true;
                leftPlaced = false;
                rightPlaced = false;
                jumpPlaced = false;
                leftClicked = false;
                rightClicked = false;
                jumpClicked = false;
                fixTiles = true;
            }
        }

        //Gravity
        playerVelY += 20.0f * fElapsedTime;

        //Drag
        if(onGround){
            playerVelX += -3.0f * playerVelX * fElapsedTime;
            if(fabs(playerVelX) < 0.01f)
                playerVelX = 0.0f;
        }

        float fNewPlayerX = fPlayerX + playerVelX * fElapsedTime;
        float fNewPlayerY = fPlayerY + playerVelY * fElapsedTime;

        if(playerVelX > 10.0f)
            playerVelX = 10.0f;
        if(playerVelX < -10.0f)
            playerVelX = -10.0f;
        if(playerVelY > 100.0f)
            playerVelY = 100.0f;
        if(playerVelY < -100.0f)
            playerVelY = -100.0f;

        if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 0.0f) == L'o')
            SetTile(fNewPlayerX + 0.0f, fNewPlayerY + 0.0f, L'.');

        if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 1.0f) == L'o')
            SetTile(fNewPlayerX + 0.0f, fNewPlayerY + 1.0f, L'.');

        if (GetTile(fNewPlayerX + 1.0f, fNewPlayerY + 0.0f) == L'o')
            SetTile(fNewPlayerX + 1.0f, fNewPlayerY + 0.0f, L'.');

        if (GetTile(fNewPlayerX + 1.0f, fNewPlayerY + 1.0f) == L'o')
            SetTile(fNewPlayerX + 1.0f, fNewPlayerY + 1.0f, L'.');
        int ingameCoins = 0;
        for (int i = 0; i < LevelOne.length();i++){
            if(LevelOne[i]=='o') {
                ingameCoins = ingameCoins+1;
            }
        }

        //Check Collision
        if(playerVelX <= 0){ //Moving Left
            if (GetTile(fNewPlayerX + 0.0f, fPlayerY + 0.0f) != '.' || GetTile(fNewPlayerX + 0.0f, fPlayerY + 0.9f) != '.'){
                fNewPlayerX = (int)fNewPlayerX + 1;
                playerVelX = 0;
            }
        }
        else{ //Moving Right
            if (GetTile(fNewPlayerX + 1.0f, fPlayerY + 0.0f) != '.' || GetTile(fNewPlayerX + 1.0f, fPlayerY + 0.9f) != '.'){
                fNewPlayerX = (int)fNewPlayerX;
                playerVelX = 0;
            }
        }

        onGround = false;
        if(playerVelY <= 0){
            if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY) != '.' || GetTile(fNewPlayerX + 0.9f, fNewPlayerY) != '.'){
                fNewPlayerY = (int)fNewPlayerY + 1;
                playerVelY = 0;
            }
        }
        else{
            if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 1.0f) != '.' || GetTile(fNewPlayerX + 0.9f, fNewPlayerY + 1.0f) != '.'){
                fNewPlayerY = (int)fNewPlayerY;
                playerVelY = 0;
                onGround = true;
            }
        }

        fPlayerX = fNewPlayerX;
        fPlayerY = fNewPlayerY;

        //Camera Properties
        cameraX = fPlayerX;
        cameraY = fPlayerY;

        //Draw Level
        int nTileWidth = 16;
        int nTileHeight = 16;
        int nVisibleTilesX = ScreenWidth() / nTileWidth;
        int nVisibleTilesY = ScreenHeight() / nTileHeight;

        //Calculate Topleft tile
        float offsetX = cameraX - (float) nVisibleTilesX / 2.0f;
        float offsetY = cameraY - (float) nVisibleTilesY / 2.0f;

        //Prevent Camera from going outside of game limits
        if(offsetX < 0) offsetX = 0;
        if(offsetY < 0) offsetY = 0;
        if(offsetX > nLevelWidth - nVisibleTilesX) offsetX = nLevelWidth - nVisibleTilesX;
        if(offsetY > nLevelHeight - nVisibleTilesY) offsetY = nLevelHeight - nVisibleTilesY;

        //Offsets for smoother movement
        float tileOffsetX = (offsetX - (int)offsetX) * nTileWidth;
        float tileOffsetY = (offsetY - (int) offsetY) * nTileHeight;

        //Draw actual tile map
        for(int i = -1; i < nVisibleTilesX+1; i++){
            for (int j = -1; j < nVisibleTilesY+1; j++){
                char tileID = GetTile(i + offsetX, j + offsetY);
                if(fixTiles && (tileID == 'l' || tileID == 'r' || tileID == 'j')){
                    tileID = '.';
                    SetTile(i+offsetX, j+offsetY, '.');
                }

                switch (tileID)
                {
                    case '.':
                        FillRect(i * nTileWidth-tileOffsetX, j * nTileHeight-tileOffsetY, nTileWidth, nTileHeight, olc::BLACK);
                        break;
                    case '#':
                        //FillRect(i * nTileWidth-tileOffsetX, j * nTileHeight-tileOffsetY, nTileWidth, nTileHeight, olc::RED);
                        DrawPartialSprite(i * nTileWidth - tileOffsetX, j * nTileHeight - tileOffsetY, spriteTiles, nDirModX * nTileWidth, nDirModY * nTileHeight, 2*nTileWidth, 2*nTileHeight);
                        break;
                    case 'l':
                        FillRect(i * nTileWidth-tileOffsetX, j * nTileHeight-tileOffsetY, nTileWidth, nTileHeight, olc::MAGENTA);
                        break;
                    case 'r':
                        FillRect(i * nTileWidth-tileOffsetX, j * nTileHeight-tileOffsetY, nTileWidth, nTileHeight, olc::BLUE);
                        break;
                    case 'j':
                        FillRect(i * nTileWidth-tileOffsetX, j * nTileHeight-tileOffsetY, nTileWidth, nTileHeight, olc::CYAN);
                        break;
                    case 'o':
                        DrawPartialSprite(i * nTileWidth - tileOffsetX, j * nTileHeight - tileOffsetY, spriteCoin, nDirModX * nTileWidth, nDirModY * nTileHeight, 2*nTileWidth, 2*nTileHeight);
                        //DrawSprite(i*nTileWidth-tileOffsetX, j*nTileHeight-tileOffsetY, spriteCoin, 1, olc::Sprite::NONE);
                        //FillRect( i * nTileWidth-tileOffsetX, j * nTileHeight - tileOffsetY, nTileWidth, nTileHeight, olc::YELLOW);
                        break;
                    default:
                        break;
                }
            }
        }

        if(fixTiles)
            fixTiles = false;

        //Draw Left Keyboard Key
        int LeftKeyX = 20;
        int LeftKeyY = 50;
        if(leftPlaced == false && leftClicked == false){
            FillRect(LeftKeyX, LeftKeyY, nTileWidth, nTileHeight, olc::MAGENTA);
        }
        if(leftPlaced == false && leftClicked == true){
            FillRect(LeftKeyX, LeftKeyY, nTileWidth, nTileHeight, olc::DARK_MAGENTA);
        }

        //Draw Right Keyboard Key
        int RightKeyX = LeftKeyX+nTileWidth+20;
        int RightKeyY = 50;
        if(rightPlaced == false && rightClicked == false){
            FillRect(RightKeyX, RightKeyY, nTileWidth, nTileHeight, olc::BLUE);
        }
        if(rightPlaced == false && rightClicked == true){
            FillRect(RightKeyX, RightKeyY, nTileWidth, nTileHeight, olc::DARK_BLUE);
        }

        //Draw Jump Keyboard Key
        int JumpKeyX = RightKeyX+nTileWidth+20;
        int JumpKeyY = 50;
        if(jumpPlaced == false && jumpClicked == false){
            FillRect(JumpKeyX, JumpKeyY, nTileWidth, nTileHeight, olc::CYAN);
        }
        if(jumpPlaced == false && jumpClicked == true){
            FillRect(JumpKeyX, JumpKeyY, nTileWidth, nTileHeight, olc::DARK_CYAN);
        }

        //Check to see if mouse clicks on keyboard key
        if(IsFocused()){
            if(GetMouse(0).bPressed){
                int clickCount = 0;
                int mouseClickX = GetMouseX();
                int mouseClickY = GetMouseY();
                if(mouseClickX >= LeftKeyX && mouseClickX <= LeftKeyX + nTileWidth && mouseClickY >= LeftKeyY && mouseClickY <= LeftKeyY+nTileHeight && leftClicked == false ){
                    leftClicked = true;
                }
                else if(mouseClickX >= RightKeyX && mouseClickX <= RightKeyX + nTileWidth && mouseClickY >= RightKeyY && mouseClickY <= RightKeyY+nTileHeight && rightClicked == false ){
                    rightClicked = true;
                }
                else if(mouseClickX >= JumpKeyX && mouseClickX <= JumpKeyX + nTileWidth && mouseClickY >= JumpKeyY && mouseClickY <= JumpKeyY+nTileHeight && jumpClicked == false){
                    jumpClicked = true;
                }
                else{
                    if(leftClicked == true){
                        if (GetTile((int) ((mouseClickX)/nTileWidth + offsetX), (int) ((mouseClickY)/nTileHeight) + offsetY) == '.') {
                            SetTile((int) ((mouseClickX)/nTileWidth + offsetX), (int) ((mouseClickY)/nTileHeight) + offsetY, 'l');
                            leftPlaced = true;
                            canMoveLeft = false;
                        }
                    }

                    else if(rightClicked == true){
                        if (GetTile((int) ((mouseClickX)/nTileWidth + offsetX), (int) ((mouseClickY)/nTileHeight) + offsetY) == '.') {
                            SetTile((int) ((mouseClickX)/nTileWidth + offsetX), (int) ((mouseClickY)/nTileHeight) + offsetY, 'r');
                            rightPlaced = true;
                            canMoveRight = false;
                        }
                    }

                    else if(jumpClicked == true){
                        if (GetTile((int) ((mouseClickX)/nTileWidth + offsetX), (int) ((mouseClickY)/nTileHeight) + offsetY) == '.') {
                            SetTile((int) ((mouseClickX)/nTileWidth + offsetX), (int) ((mouseClickY)/nTileHeight) + offsetY, 'j');
                            jumpPlaced = true;
                            canJump = false;
                        }
                    }
                    rightClicked = false;
                    jumpClicked = false;
                    leftClicked = false;
                }
            }
        }

        //Draw Amount of Coins
        coinCounter = totalcoins - ingameCoins;
        string coinString = to_string(coinCounter);
        DrawString(LeftKeyX-10,LeftKeyY+150,"Coins:"+coinString,olc::YELLOW);

        //Draw player
        DrawPartialSprite((fPlayerX - offsetX) * nTileWidth, (fPlayerY - offsetY) * nTileWidth, spriteMan, nDirModX * nTileWidth, nDirModY * nTileHeight, 1*nTileWidth, 1*nTileHeight);
        //FillRect((fPlayerX-offsetX) * nTileWidth, (fPlayerY - offsetY) * nTileHeight, nTileWidth, nTileHeight, olc::GREEN);
        //DrawPartialSprite((fPlayerX - offsetX) * nTileWidth, (fPlayerY - offsetY) * nTileWidth, spriteMan, nDirModX * nTileWidth, nDirModY * nTileHeight, nTileWidth, nTileHeight);
        SetPixelMode(olc::Pixel::NORMAL);
        return true;
    }
};


int main()
{
    Example demo;
    if (demo.Construct(256, 256, 8, 8))
        demo.Start();

    return 0;
}