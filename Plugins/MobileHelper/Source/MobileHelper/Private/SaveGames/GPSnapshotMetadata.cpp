// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveGames/GPSnapshotMetadata.h"


#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#endif

#include "Utils/GPUtils.h"

UGPSnapshotMetadata::~UGPSnapshotMetadata()
{
#if PLATFORM_ANDROID
	if (SnapshotMetadata)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		Env->DeleteGlobalRef(SnapshotMetadata);
	}
#endif
}

#if PLATFORM_ANDROID
void UGPSnapshotMetadata::Init(jobject SnapshotMetadataObject)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	SnapshotMetadata = Env->NewGlobalRef(SnapshotMetadataObject);

	jclass SnapshotMetadataClass = FAndroidApplication::FindJavaClass("com/ninevastudios/googleplay/GPSnapshotMetadata");

	GetCoverImageAspectRatioMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getCoverImageAspectRatioMethod", "()F", false);
	GetDescriptionMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getDescription", "()Ljava/lang/String;", false);
	GetDeviceNameMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getDeviceName", "()Ljava/lang/String;", false);
	GetSnapshotIdMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getSnapshotId", "()Ljava/lang/String;", false);
	GetUniqueNameMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getUniqueName", "()Ljava/lang/String;", false);
	GetLastModifiedTimestampMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getLastModifiedTimestamp", "()J", false);
	GetPlayedTimeMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getPlayedTime", "()J", false);
	GetProgressValueMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getProgressValue", "()J", false);
	GetCoverImageUriMethod = FJavaWrapper::FindMethod(Env, SnapshotMetadataClass, "getCoverImageUri", "()Ljava/lang/String;", false);
}

jobject UGPSnapshotMetadata::GetSnapshotMetadataObject()
{
	return SnapshotMetadata;
}
#endif

FString UGPSnapshotMetadata::GetDescription()
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(SnapshotMetadata, GetDescriptionMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

FString UGPSnapshotMetadata::GetDeviceName()
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(SnapshotMetadata, GetDeviceNameMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

FString UGPSnapshotMetadata::GetSnapshotId()
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(SnapshotMetadata, GetSnapshotIdMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

FString UGPSnapshotMetadata::GetUniqueName()
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(SnapshotMetadata, GetUniqueNameMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

FString UGPSnapshotMetadata::GetCoverImageUri()
{
	FString Result;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(SnapshotMetadata, GetCoverImageUriMethod));
	Result = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return Result;
}

float UGPSnapshotMetadata::GetCoverImageAspectRatio()
{
	float CoverImageAspectRatio = 0.f;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	CoverImageAspectRatio = Env->CallFloatMethod(SnapshotMetadata, GetCoverImageAspectRatioMethod);
#endif
	return CoverImageAspectRatio;
}

void UGPSnapshotMetadata::GetLastModifiedTimestamp(int64& LastModifiedTimestamp)
{
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	LastModifiedTimestamp = Env->CallLongMethod(SnapshotMetadata, GetLastModifiedTimestampMethod);
#endif
}

void UGPSnapshotMetadata::GetPlayedTime(int64& SnapshotPlayedTime)
{
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	SnapshotPlayedTime = Env->CallLongMethod(SnapshotMetadata, GetPlayedTimeMethod);
#endif
}

void UGPSnapshotMetadata::GetProgressValue(int64& ProgressValue)
{
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	ProgressValue = Env->CallLongMethod(SnapshotMetadata, GetProgressValueMethod);
#endif
}
