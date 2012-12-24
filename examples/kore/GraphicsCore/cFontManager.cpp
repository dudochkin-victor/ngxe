/*
 *  cFontManager.cpp
 *  Kore
 *
 *  Created by Sean Chapel on 11/21/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "cFontManager.h"
#include  "GraphicsCore.h"

using namespace std;

cFontManager::cFontManager()
{
}

cFontManager::~cFontManager()
{
    ReleaseAllFonts();
}

void cFontManager::SetFontPath(std::string path)
{
    m_FontPath = path;
}

std::string cFontManager::LoadFont(std::string FontName)
{
    map<string, sFont>::iterator itr = m_Fonts.find(FontName);

    //if its already loaded then just return it's name
    if( itr != m_Fonts.end() )
    {
        return FontName;
    }

    //load font texture
    uint FontTex = g_TextureManager->LoadTextureFromFile(m_FontPath + FontName + ".png");

    //if the texture didn't load return null
    if(FontTex == 0)
    {
        return NULL;
    }

    //make the font structure
    sFont font;
    font.Texture = FontTex;
    font.Name = FontName;

    //open and parse ini file
    string line;
    int numRows, numCols;

    string filename = m_FontPath + FontName + ".ini";

    //open the file
    ifstream fontIni(filename.c_str());
    if (fontIni.is_open())
    {
        //get number of rows and cols of text
        getline (fontIni,line);
        numRows = atoi(line.c_str());
        getline (fontIni,line);
        numCols = atoi(line.c_str());

        //setup the default gylph height/width
        font.GlyphWidth = (1.0f / (GLfloat) numRows) * g_TextureManager->GetTextureWidth(FontTex);
        font.GlyphHeight = (1.0f / (GLfloat) numCols) * g_TextureManager->GetTextureHeight(FontTex);
        font.Spacing = font.GlyphWidth;

        //read in all the characters
        while (! fontIni.eof() )
        {
            sGlyph glyph;
            getline (fontIni,line);

            //get gylph
            glyph.Glyph = line[0];

            //get coord
            string coords = line.substr(2, line.size() - 2);

            //get x coor
            string::size_type pos = coords.find(",", 0);
            string yCoor = coords.substr(0, pos);

            //get y coor
            string xCoor = coords.substr(pos + 1, 3);

            //make section coords
            sSection texSect;

            texSect.uMin = (GLfloat) (atoi(xCoor.c_str()) - 1) / (GLfloat) numRows;
            texSect.uMax = (GLfloat)  atoi(xCoor.c_str())      / (GLfloat) numRows;
            texSect.vMin = (GLfloat) (atoi(yCoor.c_str()) - 1) / (GLfloat) numCols;
            texSect.vMax = (GLfloat)  atoi(yCoor.c_str())      / (GLfloat) numCols;

            // make texture section
            glyph.TextureSection = g_TextureManager->CreateTextureSection(FontTex, texSect);

            font.Gylphs[glyph.Glyph] = glyph;
        }
        fontIni.close();

        m_Fonts[FontName] = font;

        return FontName;
    }
    else
    {
        cout << "Unable to open font file " << m_FontPath << FontName << ".ini" << endl;
    }

    return NULL;
}

void cFontManager::ReleaseFont(std::string Font)
{
    g_TextureManager->RemoveTexture( m_Fonts[Font].Texture);
    m_Fonts.erase( m_Fonts.find(Font) );
}

void cFontManager::ReleaseAllFonts()
{
    map<string, sFont>::iterator itr;

    for( itr  = m_Fonts.begin(); itr != m_Fonts.end(); itr++)
    {
        ReleaseFont(itr->first);
    }
}

void cFontManager::SetLetterSpacing(std::string Font, GLfloat Spacing)
{
    m_Fonts[Font].Spacing = m_Fonts[Font].GlyphWidth * Spacing;
}

void cFontManager::DrawText(std::string Font, std::string Text, GLfloat X, GLfloat Y, GLfloat Scale, GLfloat Rotation, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    //look up the font
    sFont* font = NULL;
    font = &m_Fonts[Font];

    //check again null font pointer
    if(font == NULL)
    {
        return;
    }

    //draw the text
    for(int i = 0; i < Text.size(); i++)
    {
        g_TextureManager->DrawTextureSection(font->Gylphs[Text[i]].TextureSection, X + (i * (font->Spacing) * Scale), Y, Scale, Rotation, red, green, blue, alpha);
    }
}

