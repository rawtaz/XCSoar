/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2010 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Dialogs/Internal.hpp"
#include "Screen/SingleWindow.hpp"
#include "Screen/Layout.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Init.hpp"
#include "ResourceLoader.hpp"
#include "StatusMessage.hpp"
#include "Asset.hpp"
#include "LocalPath.hpp"
#include "OS/PathName.hpp"

#include <tchar.h>
#include <stdio.h>

#ifdef WIN32
#include <shellapi.h>
#endif

const TCHAR *
GetPrimaryDataPath()
{
  return _T("");
}

const TCHAR *
GetHomeDataPath(TCHAR *buffer)
{
  return NULL;
}

Font Fonts::Map;
Font Fonts::MapBold;
Font Fonts::Title;
Font Fonts::CDI;
Font Fonts::InfoBox;

#ifndef WIN32
int main(int argc, char **argv)
#else
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
#ifdef _WIN32_WCE
        LPWSTR lpCmdLine,
#else
        LPSTR lpCmdLine2,
#endif
        int nCmdShow)
#endif
{
#ifdef WIN32
#ifndef _WIN32_WCE
  /* on Windows (non-CE), the lpCmdLine argument is narrow, and we
     have to use GetCommandLine() to get the UNICODE string */
  LPCTSTR lpCmdLine = GetCommandLine();
#endif

#ifdef _WIN32_WCE
  int argc = 2;

  WCHAR arg0[] = _T("");
  LPWSTR argv[] = { arg0, lpCmdLine, NULL };
#else
  int argc;
  LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
#endif

  ResourceLoader::Init(hInstance);

  PaintWindow::register_class(hInstance);
#endif

  if (argc < 2) {
    fprintf(stderr, "Usage: RunDialog XMLFILE\n");
    return 1;
  }

  ScreenGlobalInit screen_init;

  Layout::Initialize(320,240);
  SingleWindow main_window;
  main_window.set(_T("STATIC"), _T("RunDialog"),
                  0, 0, 320, 240);
  main_window.show();

  WndForm *form = LoadDialog(NULL, main_window, argv[1]);
  if (form == NULL) {
    fprintf(stderr, "Failed to load resource '%s'\n",
            (const char *)NarrowPathName(argv[1]));
    return 1;
  }

  form->ShowModal();
  delete form;

  return 0;
}
