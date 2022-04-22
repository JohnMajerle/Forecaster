pushd classlib
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\msbuild.exe" classlib.sln /property:Configuration=Release
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\msbuild.exe" classlib.sln /property:Configuration=Debug
popd

pushd forelib
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\msbuild.exe" forelib.sln /property:Configuration=Release
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\msbuild.exe" forelib.sln /property:Configuration=Debug
popd

pushd WinFore
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\msbuild.exe" WinFore.sln /property:Configuration=Release
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\msbuild.exe" WinFore.sln /property:Configuration=Debug
popd

