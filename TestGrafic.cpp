#include <math.h>
#include <string>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#define infinit DBL_MAX
#define epsi 1e-15
#define  MAX 100
#define MAX1 20
#define nbBtn  6
#include <stdio.h>
#include <vector>
#include<unistd.h> 
#include <SDL2/SDL.h>
#include <cstring>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


using namespace std;

int top1,top2; // top of the two stacks
const int max_stiva=100; // size of stack

double Opd[max_stiva]; // operands stack 
char Op[max_stiva]; // operators stack

char OperatiiBinare[200]="+-*/^<>=#";
char OperatiiUnare[200]="scarel";
char Operatii[200]="+-*/^<>=#scarel";

const int CANVAS_WIDTH = 800;
const int CANVAS_HEIGHT = 600;

const int left = 150, right = 650, up = 50, down = 550;
/**
 * SDL started
*/
SDL_Texture* textureZoomIn = nullptr;
SDL_Texture* textureZoomOut = nullptr;
SDL_Rect rects[nbBtn];
SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* fontNb = nullptr;
TTF_Font* fontButton = nullptr;


int Prioritate(char c) // operand precedence
{
    if(c=='(' || c==')')
        return 0;
    if(c=='+' || c=='-')
        return 1;
    if(c=='*' || c=='/')
        return 2;
    if(c=='^')
        return 3;
    if(c=='=' || c=='#' || c=='<' || c=='>')
        return 4;
    if(c=='c' || c=='s' || c=='l' || c=='e' || c=='t' || c=='a' || c=='r')
        return 5;
    return -1;
}

const double pi=3.1415926; // is given in the form of the letter q

bool DifInf(double x)
{
    return fabs(infinit-fabs(x)) > infinit / 2.0;
}

double Logaritm(double x)
{
    if (x>epsi && DifInf(x))
        return log(x);
    else
        return infinit;
}

double Exponential(double x)
{
    if (DifInf(x)) return exp(x);
    else return infinit;
}

double Inmultit(double x, double y)
{
    if (fabs(x < epsi) || fabs(y) < epsi) return 0;
        else if (DifInf(x) && DifInf(y)) return x*y;
            else return infinit;
}

double Putere(double x, double y)
{
    // float p;
    if (x==0) return 0;
    else if (y==0) return 1;
    else if (x==infinit || y==infinit) return infinit;
    else
        return pow(x,y);
}

bool Egal(double x, double y)
{
    return x==y;
}

bool Diferit(double x, double y)
{
    return x!=y;
}

bool MaiMic(double x, double y)
{
    return x < y;
}

bool MaiMare(double x, double y)
{
    return x > y;
}

double Plus(double x, double y)
{
    if (DifInf(x) && DifInf(y))  return x+y;
    else return infinit;
}

double Minus(double x, double y)
{
    if (DifInf(x) && DifInf(y))  return x-y;
    else return infinit;
}

double Impartit(double x, double y)
{
    if (fabs(y)>epsi) return x/y;
    else return infinit;
}

double Sinus(double x)
{
    if (DifInf(x))  return sin(x);
    else return infinit;
}

double Cosinus(double x)
{
    if (DifInf(x))  return cos(x);
    else return infinit;
}

double Modul(double x)
{
    if (DifInf(x)) return fabs(x);
    else return infinit;
}

double Radical(double x)
{
    if (DifInf(x) && (x>epsi)) return sqrt(x);
    else return infinit;
}

bool EsteNumar(char *sir) // function to find out if a char* is a number
{
    if(sir == NULL)
        return 0;
    
    int n = strlen(sir);

    if(n == 0)
        return 0;

    int i = 0;

    if(sir[0] == '-' || sir[0] == '+') {
        if(n == 1)
            return 0;
        i = 1;
    }

    for(; i<n; i++) {
        if(!isdigit(sir[i]) && sir[i] != '.')
            return 0;
    }

    return 1;
    
}

struct functie
{
    char expresie[300];
    char vect[MAX+1][MAX1];
    int lung; 
    float a, b, c, d; 
};
functie F; // a structure that defines a function


void depanare() //print operators and operands stack
{
    printf("Stiva operanzilor: ");
    for (int i=1; i<=top1; i++)
        printf("%f,",Opd[i]);
    printf("\nStiva operatorilor: ");
    for (int i=1; i<=top2; i++)
        printf("%c,",Op[i]);
    printf("\n");
}

int splitExpresion(functie &F){
    int n = strlen(F.expresie);
    strcpy(F.vect[0],"(");
    int j = 1;


    for(int i=0; i<n; i++) {
        if(isalpha(F.expresie[i])){
            int k = 0;
            char fun[4];
            while(isalpha(F.expresie[i])){
                F.vect[j][k] = F.expresie[i];
                i++, k++;
            }  

            i--;
            F.vect[j][k] = '\0';
        }
        else if(isdigit(F.expresie[i])) {
            int k = 0;
            while(isdigit(F.expresie[i]) || F.expresie[i] == '.') {
                F.vect[j][k] = F.expresie[i];
                i++, k++;
            }
            i--;
            F.vect[j][k]='\0';
        }
        else if(F.expresie[i] == '(' && (F.expresie[i+1] == '+' || F.expresie[i+1] == '-')) {
            if(F.expresie[i+1] == '-') {
                F.vect[j][0] = '0';
            }
            j++;
            i += 2;
            F.vect[j][0] = F.expresie[i-1];
            int k = 0;
            j++;
            if(isdigit(F.expresie[i]))
                while(isdigit(F.expresie[i]) || F.expresie[i] == '.') {
                    F.vect[j][k] = F.expresie[i];
                    i++, k++;
                }
            else {
                F.vect[j][k] = F.expresie[i];
                i++;
                k++;
            }
            F.vect[j][k]='\0';
        }
        else {
            F.vect[j][0] = F.expresie[i];
            F.vect[j][1] = '\0';
        }
        j++;
    }
     strcpy(F.vect[j],")");
    j++;
    return j;

}


enum TYPE {function, sumSub, mulDiv, nec, number, braketO, braketC, none, expo, function1};

TYPE type(char s[MAX]) {

    if(!strcmp(s, "sin") || !strcmp(s, "cos") || !strcmp(s, "ln") || !strcmp(s, "abs") || !strcmp(s, "rad"))
        return function;
    else if(!strcmp(s, "+") || !strcmp(s, "-"))
        return sumSub;
    else if(!strcmp(s, "*") || !strcmp(s, "/"))
        return mulDiv;
    else if(!strcmp(s, "X"))
        return nec;
    else if(EsteNumar(s))
        return number;
    else if(!strcmp(s, "("))
        return braketO;
    else if(!strcmp(s, ")"))
        return braketC;
    else if(!strcmp(s, "^"))
        return expo;
    else if(!strcmp(s, "-X"))
        return function1;
    return none;
}

bool isValid(functie F) { // test for an expression
    int brakets = 0;

    for(int i=1; i<F.lung-1; i++) {
        if(type(F.vect[i]) == none)
            return false;
        else if(type(F.vect[i]) == function && type(F.vect[i+1]) != braketO)
            return false;
        else if((type(F.vect[i]) == sumSub || type(F.vect[i]) == mulDiv || type(F.vect[i]) == expo || type(F.vect[i]) == function) && type(F.vect[i+1]) == braketC) {
            return false;
        }
        else if((type(F.vect[i]) == number || type(F.vect[i]) == nec) && (type(F.vect[i+1]) == number || type(F.vect[i+1]) == nec || type(F.vect[i+1]) == function)) {
            return false;
        }
        else if(type(F.vect[i]) == braketO) 
            brakets++;
        else if(type(F.vect[i]) == braketC)
            brakets--;
    }

    if(brakets == 0) 
        return true;
    else 
        return false;

}
double ValoareFunctie(functie E, double x) // function to find out the value of a function
{
    int i;
    // returns the value of the function E at point x
    double valo,x_1,x_2;

    for (i=1; i<=max_stiva; i++)
    {
        Opd[i]=0;
        Op[i]='@';
    }
    top1=0;
    top2=1;
    Op[top2]='(';
    i=0;
    while (i<=E.lung && top2>0)
    {
        i++;
        if (EsteNumar(E.vect[i]))
        {
            top1++;
            valo=atof(E.vect[i]);
            Opd[top1]=valo;
             //depanare();
        }
        else
            switch (E.vect[i][0]) {
            case 'q': top1++; Opd[top1]=pi; break; // pi
            case 'X': top1++; Opd[top1]=x; break; // varible X
            case '(': top2++; Op[top2]='('; break;
            default:
                // binary and unary operation
               while ((top2>0) && !(strchr("()",Op[top2])) &&
                   Prioritate(Op[top2])>=Prioritate(E.vect[i][0]))
                {
                    if(top1>1) x_1=Opd[top1-1];
                    x_2 = Opd[top1];

                     //depanare();
                    switch (Op[top2]) {
                        case '=': valo=Egal(x_1,x_2); break;
                        case '#': valo=Diferit(x_1,x_2); break;
                        case '<': valo=MaiMic(x_1,x_2); break;
                        case '>': valo=MaiMare(x_1,x_2); break;
                        case '+': valo=Plus(x_1,x_2); break;
                        case '-': valo=Minus(x_1,x_2); break;
                        case '*': valo=Inmultit(x_1,x_2); break;
                        case '/': valo=Impartit(x_1,x_2); break;
                        case '^': valo=Putere(x_1,x_2); break;
                        case 's': valo=Sinus(x_2); break;
                        case 'c': valo=Cosinus(x_2); break;
                        case 'l': valo=Logaritm(x_2); break;
                        case 'e': valo=Exponential(x_2); break;
                        case 'a': valo=Modul(x_2); break;
                        case 'r': valo=Radical(x_2); break;
                    } //operator[top2]
                    if (strchr(OperatiiBinare,Op[top2])) top1--;
                    if (strchr(Operatii,Op[top2])) Opd[top1]=valo;
                    top2--;
                }
            // depanare();
            if (top2>0)
                if ((Op[top2]!='(') || (strcmp(E.vect[i],")")))
                    {
                        top2++; Op[top2] = E.vect[i][0];
                    }
                else top2--;
            }
    }
    if (top2==0) return Opd[1];
    else return infinit;
}

struct ScreenPoint {
    int x, y;
    bool disc = false; // variable as a flag for the continuity and discontinuity of the function
};

vector<int> discs;

vector<ScreenPoint> computeScreenPoints() {
    
    int hScreenA =left;
    int hScreenB = right;
    int vScreenA = up;
    int vScreenB = down;
    float hRealA = F.a;
    float hRealB = F.b;
    float vRealA = F.c;
    float vRealB = F.d;
 
    discs.clear();
    vector<ScreenPoint> screenPoints;

    int hScreenSize = hScreenB - hScreenA;
    int vScreenSize = vScreenB - vScreenA;
    float hRealSize = hRealB - hRealA;
    float vRealSize = vRealB - vRealA;

    for(int i=0; i<= hScreenSize; i++) {
        //linear transformations
        float p = i*1.0/hScreenSize;
        float x = hRealA + p*hRealSize;
        float y = ValoareFunctie(F, x);

        if (abs(y) > 1e6) {
            discs.push_back(hScreenA + i);
        }
        
        if(vRealA <= y && y <= vRealB) {
            p = (y - vRealA)/vRealSize;
            int ys = vScreenA + p*vScreenSize;
            int oys = vScreenA + (vScreenB - ys);
            int xs = hScreenA + i;

           screenPoints.push_back({xs, oys});
        }
    }

    return screenPoints;
}


void init() { //funtion for initialization the main parts of the visual reprezentation (window, buttons)

    SDL_Init(SDL_INIT_VIDEO); //initialization

    window = SDL_CreateWindow("Grafic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CANVAS_WIDTH, CANVAS_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    
    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf initialization failed: %s", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    fontNb = TTF_OpenFont("Lato-Black.ttf", 10);
    if (!fontNb) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    fontButton = TTF_OpenFont("Lato-Black.ttf", 15);
    if (!fontNb) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

}

void renderText(const char* text, int x, int y, TTF_Font* font) {

    SDL_Color textColor = {255, 255, 255}; // white color (RGB)

    // render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, textColor);
    if (!textSurface) {
        SDL_Log("Failed to render text: %s", TTF_GetError());
        return;
    }

    // create texture from the rendered surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    // get the width and height of the text
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    // set the destination rectangle for rendering
    SDL_Rect dstRect = {x, y, textWidth, textHeight};

    // render the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);

    // clean up texture
    SDL_DestroyTexture(textTexture);
}



void distroy() {
    TTF_CloseFont(fontNb);
    TTF_CloseFont(fontButton);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(textureZoomIn);
    SDL_DestroyTexture(textureZoomOut);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

pair<int, int> axisOriginPoint() {
    
    int hScreenSize = right - left;
    int vScreenSize = down - up;
    float hRealSize = F.b - F.a;
    float vRealSize = F.d - F.c;

    int x0Screen = left + ((-F.a) / hRealSize)*hScreenSize;
    float p = (0 - F.c)/vRealSize;
    int ys = up + p*vScreenSize;
    int y0Screen = up + down - ys;

    return {x0Screen, y0Screen};
}


void drawAxis() {

    int hScreenA = left;
    int hScreenB = right;
    int vScreenA = up;
    int vScreenB = down;

    pair<int, int> axisOrigin = axisOriginPoint();

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    if(vScreenA <= axisOrigin.second && axisOrigin.second <= vScreenB)
        SDL_RenderDrawLine(renderer, hScreenA, axisOrigin.second, hScreenB, axisOrigin.second);
    if(hScreenA <= axisOrigin.first && axisOrigin.first <= hScreenB)
        SDL_RenderDrawLine(renderer, axisOrigin.first, vScreenA, axisOrigin.first, vScreenB);

}

bool insideDiscs(int p1, int p2) {
    for(int j=0; j<discs.size(); j++) {
            if(discs[j] > p1 && discs[j] < p2)
                return true;     
        }

    return false;
}



bool drawAsimpOrizFlag = false;
bool drawAsimpVertFlag = false;
bool showMinMaxPoints = false;
bool drawAsimpObliFlag = false;

void minMaxPoints() {

    int hScreenA = left;
    int hScreenB = right;
    float hRealA = F.a;
    float hRealB = F.b;

    float ymax = ValoareFunctie(F, F.a), ymin = ymax;
    int hScreenSize = hScreenB - hScreenA;
    float hRealSize = hRealB - hRealA;

    for(int i=0; i<=hScreenSize; i++) {
        float p = (hScreenA - i)*1.0/hScreenSize;
        float x = hRealA + p*hRealSize;
        float y = ValoareFunctie(F, x);
        if(y < ymin) ymin = y;
        if(y > ymax) ymax = y;
    }

    printf("Minimul si maximul functiei pe intervalul [%f, %f] este: %f, respectiv %f.\n", F.a, F.b, ymin, ymax);
    showMinMaxPoints = false;
}


// calculate the limit 2 times for 2 high values and then verify if difference is smaller than a certain eps(a small value)
void drawAsimptotaOrizontala() { // y = y0

    int hScreenA = left;
    int hScreenB = right;
    int vScreenA = up;
    int vScreenB = down;
    float hRealA = F.a;
    float hRealB = F.b;
    float vRealA = F.c;
    float vRealB = F.d;

    pair<int, int> axisOrigin = axisOriginPoint();

    float vRealSize = vRealB - vRealA;
    int vScreenSize = vScreenB - vScreenA;

    float epsilon = 1e-15;
    float limita1 = ValoareFunctie(F, 1e14); 
    float limita2 = ValoareFunctie(F, 1e15);
    if(limita2-limita1 < epsilon) {
        
        float p = (limita1 - vRealA)/vRealSize;
        int ys = vScreenA + p*vScreenSize;
        int oys = vScreenA + (vScreenB - ys);

        if(vScreenA <= oys && oys <= vScreenB) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawLine(renderer, hScreenA, oys, hScreenB, oys);
        }
    }
    else {
        printf("Nu exista asimptota orizontala!\n");
        drawAsimpOrizFlag = false;
    }
}


void drawAsimptotaVerticala() { //looking for points of discontinuity
    pair<int, int> axisOrigin = axisOriginPoint();

    if(discs.size() == 0){
        printf("Nu exista asimptota verticala!\n");
        drawAsimpVertFlag = false;
    }

    for(int i=0; i<discs.size(); i++) {
            if(left <= discs[i] && discs[i] <= right) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawLine(renderer, discs[i], up, discs[i], down);
            }
    }
        
}

bool isInside(pair<int, int> startPos) { //check if startPos are inside the graphic square 
    
    return left <= startPos.first && startPos.first <= right && up <= startPos.second && startPos.second <= down;
}

vector<pair<float, float>> findIntersection(float m, float n, float left, float right, float up, float down) {

    vector<pair<float, float> > intrs;
    
    float yL = m*left + n;
    if(up <= yL && yL <= down) {
        intrs.push_back({left, yL});
    }

    float yR = m*right + n;
    if(up <= yR && yR <= down) {
        intrs.push_back({right, yR});
    }

    float xL = (up - n)/m;
    if(left <= xL && xL <= right) {
        intrs.push_back({xL, up});
    }

    float xR = (down - n)/m;
    if(left <= xR && xR <= right) {
        intrs.push_back({xR, down});
    }

    return intrs;
}

void drawAsimptotaOblica() {
    // m = lim( f(x)/x ) and n = lim( f(x) - mx )
    // y = mx + n

    functie E; // f(x)/x
    strcpy(E.expresie, "(");
    strcat(E.expresie, F.expresie);
    strcat(E.expresie, ")");
    strcat(E.expresie, "*(1/X)");
    E.lung = splitExpresion(E);

    float epsilon = 1e-15;
    float limita1 = ValoareFunctie(E, 1e14);
    float limita2 = ValoareFunctie(E, 1e15);

    if(limita2 - limita1 < epsilon && limita2 > epsilon) {
        float m = limita1;
        string s = to_string(limita2);
        strcpy(E.expresie, "(");
        strcat(E.expresie, F.expresie);
        strcat(E.expresie, ")");
        strcat(E.expresie, "-");
        strcat(E.expresie, s.c_str());
        strcat(E.expresie, "*X");
        E.lung = splitExpresion(E);

        limita1 = ValoareFunctie(E, 1e14);
        limita2 = ValoareFunctie(E, 1e15); 

        vector<pair<int, int> > asimpPoints;

        if(limita2 - limita1 < epsilon) {
            float n = limita1;

            vector<pair<float, float> > intrs = findIntersection(m, n, F.a, F.b, F.c, F.d);

            if(intrs.size() == 2) {
                float x1 = intrs[0].first;
                float x2 = intrs[1].first;

                float y1 = intrs[0].second;
                float y2 = intrs[1].second;

                float p1 = (y1 - F.c) / (F.d - F.c);
                int y1s =  up + p1*(down-up);
                int y1os = up + down - y1s;

                float p2 = (y2 - F.c) / (F.d - F.c);
                int y2s = up + p2*(down-up);
                int y2os = up + down - y2s;

                p1 = (x1 - F.a)/(F.b - F.a);
                int x1s = left + p1*(right - left);

                p1 = (x2 - F.a)/(F.b - F.a);
                int x2s = left + p1*(right - left);

                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawLine(renderer, x1s, y1os, x2s, y2os);
            }

        }
        else {
            printf("Nu exista asimptota oblica!\n");
            drawAsimpObliFlag = false;
        }
    }
    else {
        printf("Nu exista asimptota oblica!\n");
        drawAsimpObliFlag = false;
    }
}

void drawFunction() {
    
    vector<ScreenPoint> points = computeScreenPoints();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i=1; i<points.size(); i++) {
        if(!insideDiscs(points[i-1].x, points[i].x) && abs(points[i-1].y - points[i].y) < (down-left)-50)
                SDL_RenderDrawLine(renderer, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
    }

}

pair<int, int> startPos = {-1, -1};

void zoomIn(float &a, float &b, float &c, float& d, float ratio) {
    a += ratio;
    b -= ratio;
    c += ratio;
    d -= ratio;
}

void zoomOut(float &a, float &b, float &c, float &d, float ratio) {
    F.a -= ratio;
    F.b += ratio;
    F.c -= ratio;
    F.d += ratio;
}

 
void handelMouse(SDL_Event  &event) { // handel the clicks of the users
    float ratio = 0.05;
    if(event.type == SDL_MOUSEBUTTONDOWN) {

        startPos = {event.motion.x, event.motion.y};
        SDL_Point startPoint = {event.motion.x, event.motion.y}; 

        for(int i=0; i<nbBtn; i++) {
            SDL_Rect rect = rects[i];
            if(SDL_PointInRect(&startPoint, &rect)){
                switch (i)
                {
                    case 0 : 
                        zoomIn(F.a, F.b, F.c, F.d, ratio);
                        
                        break;
                    case 1 :
                        zoomOut(F.a, F.b, F.c, F.d, ratio);
                        break;
                    case 2 :
                        drawAsimpOrizFlag = !drawAsimpOrizFlag;
                        break;
                    case 3 :
                        drawAsimpVertFlag = !drawAsimpVertFlag;
                        break;
                    case 4:
                        drawAsimpObliFlag = !drawAsimpObliFlag;
                        break;
                    case 5:
                        showMinMaxPoints = !showMinMaxPoints;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else if(event.type == SDL_MOUSEMOTION && startPos != make_pair(-1, -1) && isInside(startPos)) { //handle the user drag on the graphic
        pair<int, int> currentPos = {event.motion.x, event.motion.y};
        int dx = currentPos.first - startPos.first;
        int dy = currentPos.second - startPos.second;

        if(dx  < 0) F.a += ratio, F.b += ratio; //go right
        if(dx > 0) F.a -= ratio,F.b -= ratio; //go left
        if(dy < 0) F.c -= ratio, F.d -= ratio; //go up 
        if(dy > 0) F.c += ratio, F.d += ratio; //go down

        startPos = currentPos;

    }
    else if(event.type == SDL_MOUSEBUTTONUP){
        startPos = {-1, -1};
    }
}

void drawScaleXAxis() {

    pair<int, int> axisOrigin = axisOriginPoint();

    float um = (F.b - F.a)/10;

    for(float i=F.a; i<=F.b; i+=um){

        float p = (i - F.a)/(F.b-F.a);
        int xs = left + p*(right - left);

        if(isInside({xs, axisOrigin.second-5}) && isInside({xs, axisOrigin.second+5})){

            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            SDL_RenderDrawLine(renderer, xs, axisOrigin.second-5, xs, axisOrigin.second+5);

            string nr = to_string(i);
            string rounded = nr.substr(0, nr.find(".")+3);
            renderText(rounded.c_str(), xs, axisOrigin.second+10, fontNb);
        }
    }
}

void drawScaleYAxis() {
    pair<int, int> axisOrigin = axisOriginPoint();

    float um = (F.d - F.c)/10;

    for(float i=F.c; i<=F.d; i+=um) {

        float p=(i - F.c)/(F.d-F.c);
        int ys = up + p*(down-up);
        int oys = up + down - ys;

        if(isInside({axisOrigin.first-5, oys}) && isInside({axisOrigin.first+5, oys})) {

            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            SDL_RenderDrawLine(renderer, axisOrigin.first-5, oys, axisOrigin.first+5, oys);

            string nr = to_string(i);
            string rounded = nr.substr(0, nr.find(".")+3);
            renderText(rounded.c_str(), axisOrigin.first+10, oys-5, fontNb);
        }
    }
}

void createRects() {
    SDL_Rect rectZoomIn = {50, 560, 50, 30};
    SDL_Rect rectZoomOut = {700, 560, 50, 30};
    SDL_Rect rectAsimptotaOrizontala = {80, 10, 100, 35};
    SDL_Rect rectAsimptotaVerticala = {270, 10, 100, 35};
    SDL_Rect rectAsimptotaOblica = {450, 10, 100, 35};
    SDL_Rect rectMinMax = {610, 10, 100, 35};
    rects[0] = rectZoomIn;
    rects[1] = rectZoomOut;
    rects[2] = rectAsimptotaOrizontala;
    rects[3] = rectAsimptotaVerticala;
    rects[4] = rectAsimptotaOblica;
    rects[5] = rectMinMax;
}


void drawZoomInButton(SDL_Rect rect) {

    SDL_RenderCopy(renderer, textureZoomIn, nullptr, &rect);
}

void drawZoomOutButton(SDL_Rect rect) {

    SDL_RenderCopy(renderer, textureZoomOut, nullptr, &rect);
}

void asimptotaOrizontalaButton(SDL_Rect rect) {
    SDL_RenderFillRect(renderer, &rect);
    renderText("Asimptota", 95, 10, fontButton);
    renderText("Orizontala", 95, 25, fontButton);
}

void asimptotaVerticalaButton(SDL_Rect rect) {
    SDL_RenderFillRect(renderer, &rect);
    renderText("Asimptota", 285, 10, fontButton);
    renderText("Verticala", 290, 25, fontButton);
}

void asimptotaOblicaButton(SDL_Rect rect) {
    SDL_RenderFillRect(renderer, &rect);
    renderText("Asimptota", 465, 10, fontButton);
    renderText("Oblica", 470, 25, fontButton);
}

void minMaxButton(SDL_Rect rect) {
    SDL_RenderFillRect(renderer, &rect);
    renderText("Minim/", 635, 10, fontButton);
    renderText("Maxim", 635, 25, fontButton);
}

int main()
{
    //sin(X)
    //X^3/(X^2+1)
    //(1/(X-1))+1
    //1/X
    
    char s[255];
    scanf("%s", s);
    strcpy(F.expresie, s);
    F.lung=splitExpresion(F);

    F.a = -5; // horizontal range start
    F.b = 5; // horizontal range end
    F.c = -5; // vertical range start
    F.d = 5; // vertical range end

    // for(int i=0; i<F.lung; i++) {
    //     printf("%s, ", F.vect[i]);
    // }
    //printf("\n");

    init();
    
    SDL_Surface* buttonSurface = IMG_Load("zoomInButton.png");
    textureZoomIn = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_FreeSurface(buttonSurface);

    buttonSurface = IMG_Load("zoomOutButton.png");
    textureZoomOut = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_FreeSurface(buttonSurface);


    createRects(); //initialize rectangles for UI buttons and graph area

    if(!isValid(F)) { //check if exxpresion is valid
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "You have some syntax errors:  %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    bool quit = false;
    while (!quit) {

        //white rect
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        //black rect
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rect = {left, up, right - left, down - up};
        SDL_RenderFillRect(renderer, &rect);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                quit = true;
            else {
                handelMouse(event);
            }
        }
        
        drawZoomInButton(rects[0]);
        drawZoomOutButton(rects[1]);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
        asimptotaOrizontalaButton(rects[2]);
        asimptotaVerticalaButton(rects[3]);
        asimptotaOblicaButton(rects[4]);
        minMaxButton(rects[5]);

        drawScaleXAxis();
        drawScaleYAxis();
        drawAxis();

        drawFunction();

        if(drawAsimpOrizFlag)
            drawAsimptotaOrizontala();
        if(drawAsimpVertFlag)
            drawAsimptotaVerticala();
        if(showMinMaxPoints)
            minMaxPoints();
        if(drawAsimpObliFlag){
            drawAsimptotaOblica();
        }

        SDL_RenderPresent(renderer);

    }

    distroy();

    return 0;
}