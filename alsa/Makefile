all:capture.c playback.c show_pcm.c audio_test_playback.cc capture_playback.c
	gcc capture.c -lasound -o capture
	gcc playback.c -lasound -o playback
	gcc show_pcm.c -lasound -o show_pcm
	g++ audio_test_playback.cc -lasound -o test_playback 
	gcc capture_playback.c -lasound -lpthread -o  test_capture_and_playback
clean:
	rm -rf capture playback show_pcm test_playback test_capture_and_playback
