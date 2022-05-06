#include <jni.h>
#include <string>
#include "fmod.hpp"  //有hpp后缀的引用这个，这个是最新版本，加强版本
#include <unistd.h>
#include <string>

//增加命名空间
using namespace std;
using namespace FMOD;


#undef com_meishe_msvoicechange_MainActivity_MODE_NORMAL
#define com_meishe_msvoicechange_MainActivity_MODE_NORMAL 0L
#undef com_meishe_msvoicechange_MainActivity_MODE_LUOLI
#define com_meishe_msvoicechange_MainActivity_MODE_LUOLI 1L
#undef com_meishe_msvoicechange_MainActivity_MODE_DASHU
#define com_meishe_msvoicechange_MainActivity_MODE_DASHU 2L
#undef com_meishe_msvoicechange_MainActivity_MODE_JINGSONG
#define com_meishe_msvoicechange_MainActivity_MODE_JINGSONG 3L
#undef com_meishe_msvoicechange_MainActivity_MODE_GAOGUAI
#define com_meishe_msvoicechange_MainActivity_MODE_GAOGUAI 4L
#undef com_meishe_msvoicechange_MainActivity_MODE_KONGLING
#define com_meishe_msvoicechange_MainActivity_MODE_KONGLING 5L



extern "C"
JNIEXPORT jstring JNICALL
Java_com_meishe_msvoicechange_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}



extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msvoicechange_MainActivity_voiceChangeNative(JNIEnv *env, jobject thiz, jint mode,
                                                             jstring path) {

    const char *toast_content = "播放完毕";

    //c++只能使用c或者c++的内容  并能用JNI的数据，所以需要转
    //JNI的方法参数，只能使用JNI的类型，所以也需要转
    //得到传进来的音频路径
    const char *audioPath = env->GetStringUTFChars(path, NULL);

    //fmod 的音效引擎
    System *system = 0;
    //fmod 的声音
    Sound *sound = 0;
    //fmod 的音轨
    Channel *channel = 0;

    //digital signal process  数字信号处理
    DSP *dsp = 0;

    //c 的初始化方式xxx(system) 一般都是需要传递指针的，因为需要对地址进行赋值操作，，指针就是地址
    System_Create(&system);
    //初始化
    system->init(32, FMOD_INIT_NORMAL, NULL);

    //创建声音
    system->createSound(audioPath, FMOD_DEFAULT, 0, &sound);
    //播放声音
    system->playSound(sound, 0, false, &channel);

    switch (mode) {
        case com_meishe_msvoicechange_MainActivity_MODE_NORMAL:
            toast_content = "原声-->播放完毕";
            break;
        case com_meishe_msvoicechange_MainActivity_MODE_LUOLI:
            // 萝莉 音调高
            //创建dsp pitch 音调调节 默认是1  小于1音调低 大于1音调高   0.5-2（合理的调节区间）
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
            //设置pitch音调=2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,2.0f);
            //添加一个音效进去
            channel->addDSP(0,dsp);
            toast_content = "萝莉：播放完毕";
            break;
        case com_meishe_msvoicechange_MainActivity_MODE_DASHU:
            //大叔 音调低
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);

            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,0.7f);

            channel->addDSP(0,dsp);

            toast_content = "大叔：播放完毕";
            break;
        case com_meishe_msvoicechange_MainActivity_MODE_JINGSONG:
            //惊悚  多个音频轨道 拼接处理
            //音调较低
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,0.7f);
            channel->addDSP(0,dsp);

            //echo 回声
            system->createDSPByType(FMOD_DSP_TYPE_ECHO,&dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY,500);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK,35);
            channel->addDSP(1,dsp);

            // 颤抖 Tremolo frequency:频率  skew :倾斜
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO,&dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY,0.8);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW,0.8);
            channel->addDSP(2,dsp);

            toast_content = "惊悚：播放完毕";
            break;

        case com_meishe_msvoicechange_MainActivity_MODE_GAOGUAI:
            //搞怪  频率快
            float frequency;
            channel->getFrequency(&frequency);
            channel->setFrequency(frequency*1.5);
            toast_content = "搞怪：播放完毕";
            break;
        case com_meishe_msvoicechange_MainActivity_MODE_KONGLING:
            //空灵
            system->createDSPByType(FMOD_DSP_TYPE_ECHO,&dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY,200);
            //衰减 50 默认  0就没了
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK,10);

            channel->addDSP(0,dsp);

            toast_content = "空灵：播放完毕";
            break;

    }

    bool isPlay= true;
    while (isPlay){
        channel->isPlaying(&isPlay);
        usleep(1000*1000);  //单位是微秒
    }

    //进行资源释放
    sound->release();
    system->close();
    system->release();
    env->ReleaseStringUTFChars(path,audioPath);

    //回调java方法，提示用户播放完毕
    jclass _activityClazz = env->GetObjectClass(thiz);
    jmethodID _toastId = env->GetMethodID(_activityClazz, "playerEnd", "(Ljava/lang/String;)V");
    jstring _toastJString = env->NewStringUTF(toast_content);
    env->CallVoidMethod(thiz, _toastId, _toastJString);

}