package com.meishe.msvoicechange

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.TextView
import android.widget.Toast
import com.meishe.msvoicechange.databinding.ActivityMainBinding
import org.fmod.FMOD

class MainActivity : AppCompatActivity() {


    companion object {
        init {
            System.loadLibrary("msvoicechange")
        }
    }

    private val MODE_NORMAL = 0 // 正常
    private val MODE_LUOLI = 1 // 萝莉
    private val MODE_DASHU = 2 // 大叔
    private val MODE_JINGSONG = 3 // 惊悚
    private val MODE_GAOGUAI = 4 // 搞怪
    private val MODE_KONGLING = 5 // 空灵

    // 播放的路径
    private val PATH = "file:///android_asset/wy多幸运-0-20.mp3"

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        FMOD.init(this);

    }


    // 六个 点击事件
    fun onFix(view: View) {
        when (view.id) {
            R.id.btn_normal -> voiceChangeNative(MODE_NORMAL, PATH)
            R.id.btn_luoli -> voiceChangeNative(MODE_LUOLI, PATH)
            R.id.btn_dashu -> voiceChangeNative(MODE_DASHU, PATH)
            R.id.btn_jingsong -> voiceChangeNative(MODE_JINGSONG, PATH)
            R.id.btn_gaoguai -> voiceChangeNative(MODE_GAOGUAI, PATH)
            R.id.btn_kongling -> voiceChangeNative(MODE_KONGLING, PATH)
        }
    }


    //////////////////////////////////native//////////////////////////////

    external fun stringFromJNI(): String

    /**
     * path:音频文件的路径
     * mode：变声的类型：原声、萝莉、大叔等
     */
    external fun voiceChangeNative(mode: Int, path: String)

    // 给native调用的   方法签名： (Ljava/lang/String;)V
    fun playerEnd(nativeMessageContent: String) {
        Toast.makeText(this, "" + nativeMessageContent, Toast.LENGTH_SHORT).show()
    }


    override fun onDestroy() {
        super.onDestroy()
        FMOD.close();
    }
}