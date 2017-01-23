all:capture.c playback.c show_pcm.c audio_test_playback.cc 
	gcc capture.c -lasound -o capture
	gcc playback.c -lasound -o playback
	gcc show_pcm.c -lasound -o show_pcm
	g++ audio_test_playback.cc -lasound -o test_playback 
clean:
	rm -rf capture playback show_pcm test_playback
