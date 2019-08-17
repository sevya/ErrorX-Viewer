install_name_tool -change lib/liberrorx.dylib "@loader_path/../Resources/lib/liberrorx.dylib" $1.app/Contents/MacOS/$1
