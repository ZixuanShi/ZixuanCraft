// Copyright Epic Games, Inc. All Rights Reserved.

#include "Auth/GPAccount.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#endif

UGPAccount::~UGPAccount()
{
#if PLATFORM_ANDROID
	if (Account)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		Env->DeleteGlobalRef(Account);
	}
#endif
}

#if PLATFORM_ANDROID
void UGPAccount::Init(jobject AccountObject)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	Account = Env->NewGlobalRef(AccountObject);

	jclass AccountClass = FAndroidApplication::FindJavaClass("com/ninevastudios/googleplay/GPAccount");
	GetDisplayNameMethod = FJavaWrapper::FindMethod(Env, AccountClass, "getDisplayName", "()Ljava/lang/String;", false);
	GetAccountIdMethod = FJavaWrapper::FindMethod(Env, AccountClass, "getAccountId", "()Ljava/lang/String;", false);
	GetEmailMethod = FJavaWrapper::FindMethod(Env, AccountClass, "getEmail", "()Ljava/lang/String;", false);
	GetIdTokenMethod = FJavaWrapper::FindMethod(Env, AccountClass, "getIdToken", "()Ljava/lang/String;", false);
}
#endif

FString UGPAccount::AccountName() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring NameString = static_cast<jstring>(Env->CallObjectMethod(Account, GetDisplayNameMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, NameString);
#endif
	return Result;
}

FString UGPAccount::AccountId() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(Account, GetAccountIdMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

FString UGPAccount::AccessToken() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring EmailString = static_cast<jstring>(Env->CallObjectMethod(Account, GetIdTokenMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, EmailString);
#endif
	return Result;
}

FString UGPAccount::Email() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(Account, GetEmailMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}
