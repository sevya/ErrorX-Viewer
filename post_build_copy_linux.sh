patchelf --replace-needed liberrorx.so /usr/local/lib/liberrorx.so ErrorX_Viewer
# rm -f ErrorX_Viewer.AppDir/AppRun
# ~/linuxdeploy-x86_64.AppImage --appdir ErrorX_Viewer.AppImage --library lib/liberrorx.so --executable ErrorX_Viewer --desktop-file ErrorX_Viewer.desktop --icon-file helix_icon.png --output appimage
# ~/linuxdeploy-x86_64.AppImage --appdir ErrorX_Viewer.AppDir --executable ErrorX_Viewer --desktop-file ErrorX_Viewer.desktop --icon-file helix_icon.png --output appimage
cp ErrorX_Viewer ErrorX-Viewer_1.0.0/usr/local/bin/
# dpkg-deb --build ErrorX-Viewer_1.0.0