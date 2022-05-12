// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveGames/GPSnapshot.h"
#include "SaveGames/GPSnapshotMetadata.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#endif

#include "Utils/GPUtils.h"

UGPSnapshot::~UGPSnapshot()
{
#if PLATFORM_ANDROID
	if (SnapshotObject)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		Env->DeleteGlobalRef(SnapshotObject);
	}
#endif
}

#if PLATFORM_ANDROID
void UGPSnapshot::Init(jobject Snapshot)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	SnapshotObject = Env->NewGlobalRef(Snapshot);

	jclass SnapshotClass = FAndroidApplication::FindJavaClass("com/ninevastudios/googleplay/GPSnapshot");

	GetMetadataMethod = FJavaWrapper::FindMethod(Env, SnapshotClass, "getSnapshotMetadata", "()Lcom/ninevastudios/googleplay/GPSnapshotMetadata;", false);
}

jobject UGPSnapshot::GetSnapshotObject()
{
	return SnapshotObject;
}
#endif

UGPSnapshotMetadata* UGPSnapshot::GetMetadata() const
{
	UGPSnapshotMetadata* SnapshotMetadata = NewObject<UGPSnapshotMetadata>();
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jobject SnapshotMetadataObject = Env->CallObjectMethod(SnapshotObject, GetMetadataMethod);
	SnapshotMetadata->Init(SnapshotMetadataObject);
#endif
	return SnapshotMetadata;
}
