#include <alsa/asoundlib.h>
#include <iostream>
#include <stdio.h>

int main()
{
    std::cout <<"hello alsa";

    snd_pcm_t *pcm_handle;

    snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

    snd_pcm_hw_params_t *hwparams;

    char* pcm_name;

    pcm_name = strdup("hw:0,0");

    snd_pcm_hw_params_alloca(&hwparams);

    if(snd_pcm_open(&pcm_handle,pcm_name,stream,0) < 0)
    {
        std::cout <<"open pcm device "<<pcm_name <<"error"<<std::endl;
        return -1;
    }

    if(snd_pcm_hw_params_any(pcm_handle,hwparams) <0)
    {
        std::cout<<"can not configure this pcm device"<<std::endl;
        return -1;
    }

    unsigned int rate = 44100; /* Sample rate */
    unsigned int exact_rate;   /* Sample rate returned by */
                      /* snd_pcm_hw_params_set_rate_near */
    int dir;          /* exact_rate == rate --> dir = 0 */
                      /* exact_rate < rate  --> dir = -1 */
                      /* exact_rate > rate  --> dir = 1 */
    int periods = 2;       /* Number of periods */
    snd_pcm_uframes_t periodsize = 8192; /* Periodsize (bytes) */
    /* Set access type. This can be either    */
    /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
    /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
    /* There are also access types for MMAPed */
    /* access, but this is beyond the scope   */
    /* of this introduction.                  */
    if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
        fprintf(stderr, "Error setting access.\n");
        return(-1);

    }
    /* Set sample format */
    if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
        fprintf(stderr, "Error setting format.\n");
        return(-1);

    }

    /* Set sample rate. If the exact rate is not supported */
    /* by the hardware, use nearest possible rate.         */
    exact_rate = rate;
    if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0) < 0) {
        fprintf(stderr, "Error setting rate.\n");
        return(-1);

    }
    if (rate != exact_rate) {
        fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n ==> Using %d Hz instead.\n", rate, exact_rate);

    }

    /* Set number of channels */
    if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 2) < 0) {
        fprintf(stderr, "Error setting channels.\n");
        return(-1);

    }

    /* Set number of periods. Periods used to be called fragments. */
    if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0) {
        fprintf(stderr, "Error setting periods.\n");
        return(-1);

    }

    /* Set buffer size (in frames). The resulting latency is given by */
    /* latency = periodsize * periods / (rate * bytes_per_frame)     */
    if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, (periodsize * periods)>>2) < 0) {
        fprintf(stderr, "Error setting buffersize.\n");
        return(-1);

    }
    /* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    if (snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
        fprintf(stderr, "Error setting HW params.\n");
        return(-1);

    }

    unsigned char *data;
    int pcmreturn, l1, l2;
    short s1, s2;
    int frames;

    data = (unsigned char *)malloc(periodsize);
    frames = periodsize >> 2;
    for(l1 = 0; l1 < 100; l1++) {
        for(l2 = 0; l2 < frames; l2++) {
            s1 = (l2 % 128) * 100 - 5000;
            s2 = (l2 % 256) * 100 - 5000;
            data[4*l2] = (unsigned char)s1;
            data[4*l2+1] = s1 >> 8;
            data[4*l2+2] = (unsigned char)s2;
            data[4*l2+3] = s2 >> 8;

        }
        while ((pcmreturn = snd_pcm_writei(pcm_handle, data, frames)) < 0) {
            snd_pcm_prepare(pcm_handle);
            fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");

        }

    }
    /* Stop PCM device and drop pending frames */
//    snd_pcm_drop(pcm_handle);

    /* Stop PCM device after pending frames have been played */
    snd_pcm_drain(pcm_handle);

    return 0;
}
