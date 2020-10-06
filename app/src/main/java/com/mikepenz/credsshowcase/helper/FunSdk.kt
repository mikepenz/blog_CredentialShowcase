package com.mikepenz.credsshowcase.helper

import android.util.Log

object FunSdk {
    fun init(key: String, secret: String) {
        Log.d("FunSdk", "key: $key, secret: $secret")
    }
}