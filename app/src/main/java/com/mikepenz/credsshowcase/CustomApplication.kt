package com.mikepenz.credsshowcase

import android.app.Application
import com.mikepenz.credsshowcase.helper.FunSdk

class CustomApplication : Application() {

    init {
        System.loadLibrary("protected")
    }

    private external fun getSdkKey(): String

    private external fun getSdkSecret(): String

    override fun onCreate() {
        super.onCreate()

        // source
        FunSdk.init(getSdkKey(), getSdkSecret())
    }
}