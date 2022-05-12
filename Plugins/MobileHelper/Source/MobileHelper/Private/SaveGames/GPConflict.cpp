// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveGames/GPConflict.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#endif

#include "Utils/GPUtils.h"

UGPConflict::~UGPConflict()
{
#if PLATFORM_ANDROID
	if (ConflictObject)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		Env->DeleteGlobalRef(ConflictObject);
	}
#endif
}

#if PLATFORM_ANDROID
void UGPConflict::Init(jobject Conflict)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	ConflictObject = Env->NewGlobalRef(Conflict);

	jclass ConflictClass = FAndroidApplication::FindJavaClass("com/ninevastudios/googleplay/GPConflict");

	GetSnapshotConflictIdMethod = FJavaWrapper::FindMethod(Env, ConflictClass, "getSnapshotConflictId", "()Ljava/lang/String;", false);
	GetLocalSnapshotMethod = FJavaWrapper::FindMethod(Env, ConflictClass, "getLocalSnapshot", "()Lcom/ninevastudios/googleplay/GPSnapshot;", false);
	GetServerSnapshotMethod = FJavaWrapper::FindMethod(Env, ConflictClass, "getServerSnapshot", "()Lcom/ninevastudios/googleplay/GPSnapshot;", false);
}

jobject UGPConflict::GetConflictObject()
{
	return ConflictObject;
}
#endif

FString UGPConflict::GetSnapshotconflictId() const
{
	FString ConflictId;
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring IdString = static_cast<jstring>(Env->CallObjectMethod(ConflictObject, GetSnapshotConflictIdMethod));
	ConflictId = FJavaHelper::FStringFromLocalRef(Env, IdString);
#endif
	return ConflictId;
}

UGPSnapshot* UGPConflict::GetLocalSnapshot() const
{
	UGPSnapshot* LocalSnapshot = NewObject<UGPSnapshot>();
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	LocalSnapshot->Init(Env->CallObjectMethod(ConflictObject, GetLocalSnapshotMethod));
#endif
	return LocalSnapshot;
}

UGPSnapshot* UGPConflict::GetServerSnapshot() const
{
	UGPSnapshot* ServerSnapshot = NewObject<UGPSnapshot>();
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	ServerSnapshot->Init(Env->CallObjectMethod(ConflictObject, GetLocalSnapshotMethod));
#endif
	return ServerSnapshot;
}