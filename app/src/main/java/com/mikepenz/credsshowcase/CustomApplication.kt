package com.mikepenz.credsshowcase

import android.app.Application
import com.mikepenz.credsshowcase.helper.FunSdk

class CustomApplication : Application() {

    override fun onCreate() {
        super.onCreate()

        // source
        FunSdk.init("amazing-key", "super-secure-secret")

        // resources
        FunSdk.init(getString(R.string.key), getString(R.string.secret))
    }
}