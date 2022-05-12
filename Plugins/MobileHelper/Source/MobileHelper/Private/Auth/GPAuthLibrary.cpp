// Copyright Epic Games, Inc. All Rights Reserved.

#include "Auth/GPAuthLibrary.h"

#include "Async/Async.h"
#include "Auth/GPAccount.h"
#include "Auth/GPPlayer.h"
#include "MobileHelper.h"
#include "MobileHelperSettings.h"

#if PLATFORM_ANDROID
#include "Android/Utils/GPMethodCallUtils.h"
#endif

UGPAuthLibrary::FGPLoginDelegate UGPAuthLibrary::OnLoginSuccessDelegate;
UGPAuthLibrary::FGPDelegate UGPAuthLibrary::OnLoginCancelDelegate;
UGPAuthLibrary::FGPDelegate UGPAuthLibrary::OnLogoutSuccessDelegate;
UGPAuthLibrary::FGPErrorDelegate UGPAuthLibrary::OnLoginErrorDelegate;

static const ANSICHAR* GPAuthClassName = "com/ninevastudios/googleplay/GPAuth";

void UGPAuthLibrary::Login(bool bDoSilentLogin, const FGPLoginDelegate& OnLoginSuccess, const FGPDelegate& OnLoginCancel, const FGPErrorDelegate& OnLoginError, const FGPPermissionFlags& PermissionFlags)
{
	UGPAuthLibrary::OnLoginSuccessDelegate = OnLoginSuccess;
	UGPAuthLibrary::OnLoginCancelDelegate = OnLoginCancel;
	UGPAuthLibrary::OnLoginErrorDelegate = OnLoginError;

#if PLATFORM_ANDROID
	UMobileHelperSettings* Settings = FMobileHelperModule::Get().GetSettings();
	auto ClientID = FJavaClassObject::GetJString(Settings->ClientID);
	GPMethodCallUtils::CallStaticVoidMethod(GPAuthClassName, "login", "(Landroid/app/Activity;ZZZZZZLjava/lang/String;)V",
		FJavaWrapper::GameActivityThis, bDoSilentLogin,
		PermissionFlags.bId, PermissionFlags.bProfile, PermissionFlags.bEmail, PermissionFlags.bIdToken, PermissionFlags.bCloudSave, *ClientID);
#endif
}

void UGPAuthLibrary::Logout(const FGPDelegate& OnLogoutSuccess)
{
	UGPAuthLibrary::OnLogoutSuccessDelegate = OnLogoutSuccess;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod(GPAuthClassName, "logout", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis);
#endif
}

void UGPAuthLibrary::RevokeAccess()
{
#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod(GPAuthClassName, "revoke", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis);
#endif
}

bool UGPAuthLibrary::IsLoggedIn()
{
	bool bResult = false;
#if PLATFORM_ANDROID
	bResult = GPMethodCallUtils::CallStaticBoolMethod(GPAuthClassName, "isLoggedIn", "(Landroid/app/Activity;)Z", FJavaWrapper::GameActivityThis);
#endif
	return bResult;
}

#if PLATFORM_ANDROID
JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onLoginSuccessCallback(
	JNIEnv* env, jclass clazz, jobject account, jobject player)
{
	UGPAccount* Account = NewObject<UGPAccount>();
	Account->Init(account);

	UGPPlayer* Player = NewObject<UGPPlayer>();
	Player->Init(player);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPAuthLibrary::OnLoginSuccessDelegate.ExecuteIfBound(Account, Player);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onLoginCancelCallback(
	JNIEnv* env, jclass clazz)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPAuthLibrary::OnLoginCancelDelegate.ExecuteIfBound();
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onLoginErrorCallback(
	JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPAuthLibrary::OnLoginErrorDelegate.ExecuteIfBound(ErrorMessage);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onLogoutSuccessCallback(
	JNIEnv* env, jclass clazz)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPAuthLibrary::OnLogoutSuccessDelegate.ExecuteIfBound();
	});
}
#endif
