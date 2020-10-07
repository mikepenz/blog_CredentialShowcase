package com.mikepenz.credsshowcase

import android.app.Application
import android.content.Context
import com.mikepenz.credsshowcase.helper.FunSdk

class CustomApplication : Application() {

    init {
        System.loadLibrary("protected")
    }

    private external fun getSdkKey(context: Context): String

    private external fun getSdkSecret(context: Context): String

    override fun onCreate() {
        super.onCreate()

        // source
        FunSdk.init(getSdkKey(this), getSdkSecret(this))
    }
}