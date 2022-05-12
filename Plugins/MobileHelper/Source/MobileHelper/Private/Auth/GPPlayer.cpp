// Copyright Epic Games, Inc. All Rights Reserved.

#include "Auth/GPPlayer.h"


#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#endif

#include "Utils/GPUtils.h"


UGPPlayer::~UGPPlayer()
{
#if PLATFORM_ANDROID
	if (Player)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		Env->DeleteGlobalRef(Player);
	}
#endif
}

#if PLATFORM_ANDROID
void UGPPlayer::Init(jobject PlayerObject)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	Player = Env->NewGlobalRef(PlayerObject);

	jclass PlayerClass = FAndroidApplication::FindJavaClass("com/ninevastudios/googleplay/GPPlayer");

	jmethodID InitCallbacksMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "initCallbacks", "(J)V", false);
	Env->CallVoidMethod(Player, InitCallbacksMethod, (jlong)this);

	GetDisplayNameMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getDisplayName", "()Ljava/lang/String;", false);
	GetPlayerIdMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getPlayerId", "()Ljava/lang/String;", false);
	GetTitleMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getTitle", "()Ljava/lang/String;", false);
	GetRetrievedTimestampMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getTimestampSeconds", "()J", false);
	GetBannerImageMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getBannerImage", "(Landroid/app/Activity;Z)V", false);
	GetIconImageMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getIconImage", "(Landroid/app/Activity;Z)V", false);
	GetFriendsListVisibilityStatusMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getFriendsListVisibilityStatus", "()I", false);
	GetPlayerFriendStatusMethod = FJavaWrapper::FindMethod(Env, PlayerClass, "getPlayerFriendStatus", "()I", false);
}
#endif

FString UGPPlayer::PlayerName() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring NameString = static_cast<jstring>(Env->CallObjectMethod(Player, GetDisplayNameMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, NameString);
#endif
	return Result;
}

FString UGPPlayer::PlayerId() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(Player, GetPlayerIdMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

FString UGPPlayer::PlayerTitle() const
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring TitleString = static_cast<jstring>(Env->CallObjectMethod(Player, GetTitleMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, TitleString);
#endif
	return Result;
}

FDateTime UGPPlayer::GetPlayerRetrievedTimestamp() const
{
	FDateTime Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	long Timestamp = Env->CallLongMethod(Player, GetRetrievedTimestampMethod);
	Result = FDateTime::FromUnixTimestamp(Timestamp);
#endif
	return Result;
}

void UGPPlayer::GetBannerImage(const FGPOnImageReadyDelegate& OnImageReady, EBannerImageOrientation Orientation)
{
	OnBannerImageReadyDelegate = OnImageReady;

#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	Env->CallVoidMethod(Player, GetBannerImageMethod, FJavaWrapper::GameActivityThis,
		Orientation == EBannerImageOrientation::Portrait ? true : false);
#endif
}

void UGPPlayer::GetIconImage(const FGPOnImageReadyDelegate& OnImageReady, bool bIsHiRes)
{
	OnIconImageReadyDelegate = OnImageReady;

#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	Env->CallVoidMethod(Player, GetIconImageMethod, FJavaWrapper::GameActivityThis, bIsHiRes);
#endif
}

EFriendsListVisibilityStatus UGPPlayer::GetFriendsListVisibilityStatus() const
{
	int VisibilityStatusValue = 0;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	VisibilityStatusValue = Env->CallIntMethod(Player, GetFriendsListVisibilityStatusMethod);
#endif

	switch (VisibilityStatusValue)
	{
		case 0:
			return EFriendsListVisibilityStatus::Unknown;
		case 1:
			return EFriendsListVisibilityStatus::Visible;
		case 2:
			return EFriendsListVisibilityStatus::RequestRequired;
		case 3:
			return EFriendsListVisibilityStatus::FeatureUnavailable;
		default:
			return EFriendsListVisibilityStatus::Unknown;
	}
}

EPlayerFriendStatus UGPPlayer::GetPlayerFriendStatus() const
{
	int FriendStatusValue = 0;

#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	FriendStatusValue = Env->CallIntMethod(Player, GetPlayerFriendStatusMethod);
#endif
	switch (FriendStatusValue)
	{
		case -1:
			return EPlayerFriendStatus::Unknown;
		case 0:
			return EPlayerFriendStatus::NoRelationship;
		case 4:
			return EPlayerFriendStatus::Friend;
		default:
			return EPlayerFriendStatus::Unknown;
	}
}

#if PLATFORM_ANDROID

JNI_METHOD void Java_com_ninevastudios_googleplay_GPPlayer_onBannerReady(JNIEnv* env, jclass clazz,
	jlong objAddr, jbyteArray buffer, int Width, int Height)
{
	UGPPlayer* Player = reinterpret_cast<UGPPlayer*>(objAddr);
	TArray<uint8> ByteArray = GPJavaConvertor::ToByteArray(static_cast<jbyteArray>(buffer));
	AsyncTask(ENamedThreads::GameThread, [=]() {
		Player->OnBannerImageReady(GPUtils::TextureFromByteArray(ByteArray, Width, Height));
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPPlayer_onIconReady(JNIEnv* env, jclass clazz,
	jlong objAddr, jbyteArray buffer, int Width, int Height)
{
	UGPPlayer* Player = reinterpret_cast<UGPPlayer*>(objAddr);
	TArray<uint8> ByteArray = GPJavaConvertor::ToByteArray(static_cast<jbyteArray>(buffer));
	AsyncTask(ENamedThreads::GameThread, [=]() {
		Player->OnIconImageReady(GPUtils::TextureFromByteArray(ByteArray, Width, Height));
	});
}

#endif
