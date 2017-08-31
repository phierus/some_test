
@echo off

for /f %%i in ('dir before_chop/a-d/b/p *.mp3') do (
	:: ffprobe.exe -i "before_chop\%%i" -show_entries format=duration -v quiet -of csv="p=0"
	ffmpeg.exe -ss 27 -i "before_chop\%%i" -acodec copy "out\%%i" 
	:: echo "before_chop\%%i"
	:: echo "out\%%i"
)
pause
