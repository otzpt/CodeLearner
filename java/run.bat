@echo off
rem Launches the Java course on Windows. Same job as ./run on Linux -- see
rem that file's comment for why a wrapper is needed instead of pointing the
rem launcher straight at a .java file. Untested on Windows, same as the rest
rem of this repo's Windows path.
cd /d "%~dp0src"
java Main.java
