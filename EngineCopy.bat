
cd..
cd..

xcopy .\GameEngine\Include\*.h .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.inl .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.hpp .\Engine\Include\ /s /d /y

xcopy .\GameEngine\Bin\*.lib .\Engine\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\GameClient\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\GameEditor\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\Engine\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\GameClient\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\GameEditor\Bin\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\Engine\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\GameClient\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\GameEditor\Bin\Resource\ /s /d /y
