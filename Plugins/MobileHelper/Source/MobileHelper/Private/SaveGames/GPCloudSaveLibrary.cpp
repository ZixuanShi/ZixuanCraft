// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveGames/GPCloudSaveLibrary.h"

#include "Async/Async.h"
#include "Auth/GPAccount.h"
#include "Auth/GPPlayer.h"
#include "MobileHelper.h"
#include "MobileHelperSettings.h"
#include "SaveGames/GPSnapshot.h"
#include "SaveGames/GPSnapshotMetadata.h"

#if PLATFORM_ANDROID
#include "Android/Utils/GPMethodCallUtils.h"
#endif

// Show Saved Games UI
UGPCloudSaveLibrary::FGPDelegate UGPCloudSaveLibrary::OnAddNewSnapshotDelegate;
UGPCloudSaveLibrary::FGPOnSnapshotMetadataDelegate UGPCloudSaveLibrary::OnViewSnapshotMetadataDelegate;
UGPCloudSaveLibrary::FGPDelegate UGPCloudSaveLibrary::OnCloudSaveCanceledDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnCloudSaveErrorDelegate;

//Open Snapshot With File Name
UGPCloudSaveLibrary::FGPOnSnapshotOpenSuccessDelegate UGPCloudSaveLibrary::OnSnapshotOpenWithFileNameSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnOpenSnapshotWithFileNameErrorDelegate;
UGPCloudSaveLibrary::FGPOnConflictDelegate UGPCloudSaveLibrary::OnOpenSnapshotByFileNameConflictDelegate;

//Open Snapshot With Metadata
UGPCloudSaveLibrary::FGPOnSnapshotOpenSuccessDelegate UGPCloudSaveLibrary::OnSnapshotOpenWithMetadataSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnOpenSnapshotWithMetadataErrorDelegate;
UGPCloudSaveLibrary::FGPOnConflictDelegate UGPCloudSaveLibrary::OnOpenSnapshotWithMetadataConflictDelegate;

//Write Snapshot
UGPCloudSaveLibrary::FGPOnSnapshotMetadataDelegate UGPCloudSaveLibrary::OnWriteSnapshotSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnWriteSnapshotFailureDelegate;

//Read Snapshot
UGPCloudSaveLibrary::FGPOnSnapshotReadSuccessDelegate UGPCloudSaveLibrary::OnSnapshotReadSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnReadSnapshotFailureDelegate;

//Get Max Cover Image Size
UGPCloudSaveLibrary::FGPOnGetMaxSizeDelegate UGPCloudSaveLibrary::OnGetMaxCoverImageSizeSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnGetMaxCoverImageFailureDelegate;

//Get Max Data Size
UGPCloudSaveLibrary::FGPOnGetMaxSizeDelegate UGPCloudSaveLibrary::OnGetMaxDataSizeSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnGetMaxDataFailureDelegate;

// Delete Snapshot
UGPCloudSaveLibrary::FGPOnDeleteSnapshotDelegate UGPCloudSaveLibrary::OnDeleteSnapshotSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnDeleteSnapshotFailureDelegate;

// Load Snapshot Metadata
UGPCloudSaveLibrary::FGPOnLoadSnapshotMetadataDelegate UGPCloudSaveLibrary::OnLoadSnapshotMetadataDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnLoadSnapshotMetadataFailureDelegate;

//Resolve Conflict
UGPCloudSaveLibrary::FGPOnSnapshotOpenSuccessDelegate UGPCloudSaveLibrary::OnResolveConflictSuccessDelegate;
UGPCloudSaveLibrary::FGPOnErrorDelegate UGPCloudSaveLibrary::OnResolveConflictFailureDelegate;
UGPCloudSaveLibrary::FGPOnConflictDelegate UGPCloudSaveLibrary::OnResolveConflictConflictDelegate;

void UGPCloudSaveLibrary::ShowSavedGamesUI(const FString& Title, bool AllowAddButton, bool AllowDelete, int MaxSnapshots, 	
	const FGPDelegate& OnAddNewSnapshot, 
	const FGPOnSnapshotMetadataDelegate& OnViewSnapshotMetadata, 
	const FGPDelegate& OnCancel, 
	const FGPOnErrorDelegate& OnError)
{
	UGPCloudSaveLibrary::OnAddNewSnapshotDelegate = OnAddNewSnapshot;
	UGPCloudSaveLibrary::OnViewSnapshotMetadataDelegate = OnViewSnapshotMetadata;
	UGPCloudSaveLibrary::OnCloudSaveCanceledDelegate = OnCancel;
	UGPCloudSaveLibrary::OnCloudSaveErrorDelegate = OnError;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "showSavedGamesUI", "(Landroid/app/Activity;Ljava/lang/String;ZZI)V", FJavaWrapper::GameActivityThis, GPJavaConvertor::GetJavaString(Title), AllowAddButton, AllowDelete, MaxSnapshots);
#endif
}

int UGPCloudSaveLibrary::GetResolutionPolicy(EConflictResolutionPolicy ResolutionPolicy)
{
	switch (ResolutionPolicy)
	{
		case EConflictResolutionPolicy::LongestPlaytime:
			return 1;
		case EConflictResolutionPolicy::LastKnownGood:
			return 2;
		case EConflictResolutionPolicy::MostRecentlyModified:
			return 3;
		case EConflictResolutionPolicy::HighestProgress:
			return 4;
		default:
			return -1;
	}
}

void UGPCloudSaveLibrary::OpenSnapshotWithFileName(const FString& FileName, bool CreateIfNotFound, EConflictResolutionPolicy ConflictPolicy,
	const FGPOnSnapshotOpenSuccessDelegate& OnSnapshotOpenSuccess, 
	const FGPOnErrorDelegate& OnOpenSnapshotError, 
	const FGPOnConflictDelegate& OnOpenSnapshotByFileNameConflict)
{
	UGPCloudSaveLibrary::OnSnapshotOpenWithFileNameSuccessDelegate = OnSnapshotOpenSuccess;
	UGPCloudSaveLibrary::OnOpenSnapshotWithFileNameErrorDelegate = OnOpenSnapshotError;
	UGPCloudSaveLibrary::OnOpenSnapshotByFileNameConflictDelegate = OnOpenSnapshotByFileNameConflict;

	int CurrentConflictPolicy = GetResolutionPolicy(ConflictPolicy);
	
#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "openSnapshotWithFileName", "(Landroid/app/Activity;Ljava/lang/String;ZI)V", FJavaWrapper::GameActivityThis, GPJavaConvertor::GetJavaString(FileName), CreateIfNotFound, CurrentConflictPolicy);
#endif
}

void UGPCloudSaveLibrary::OpenSnapshotWithMetadata(UGPSnapshotMetadata* SnapshotMetadata, EConflictResolutionPolicy ConflictPolicy, const FGPOnSnapshotOpenSuccessDelegate& OnSnapshotOpenWithMetadataSuccess, 	const FGPOnErrorDelegate& OnOpenSnapshotWithMetadataError, 	const FGPOnConflictDelegate& OnOpenSnapshotWithMetadataConflict)
{
	UGPCloudSaveLibrary::OnSnapshotOpenWithMetadataSuccessDelegate = OnSnapshotOpenWithMetadataSuccess;
	UGPCloudSaveLibrary::OnOpenSnapshotWithMetadataErrorDelegate = OnOpenSnapshotWithMetadataError;
	UGPCloudSaveLibrary::OnOpenSnapshotWithMetadataConflictDelegate = OnOpenSnapshotWithMetadataConflict;

	const int CurrentConflictPolicy = GetResolutionPolicy(ConflictPolicy);
#if PLATFORM_ANDROID
	
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "openSnapshotWithMetadata", "(Landroid/app/Activity;Lcom/ninevastudios/googleplay/GPSnapshotMetadata;I)V", FJavaWrapper::GameActivityThis, SnapshotMetadata->GetSnapshotMetadataObject(), CurrentConflictPolicy);
#endif
}

void UGPCloudSaveLibrary::WriteSnapshot(UGPSnapshot* Snapshot, 	TArray<uint8> Data, UTexture2D* CoverImage, const FString& Description,	int64 PlayedTimeMillis,	int64 ProgressValue, const FGPOnSnapshotMetadataDelegate& OnWriteSnapshotSuccess, const FGPOnErrorDelegate& OnWriteSnapshotFailure)
{
	UGPCloudSaveLibrary::OnWriteSnapshotSuccessDelegate = OnWriteSnapshotSuccess;
	UGPCloudSaveLibrary::OnWriteSnapshotFailureDelegate = OnWriteSnapshotFailure;
#if PLATFORM_ANDROID
	int Width = -1;
	int Height = -1;
	jbyteArray jArray = nullptr;
	if (CoverImage != nullptr)
	{
		Width = CoverImage->GetSizeX();
		Height = CoverImage->GetSizeY();
		TArray<uint8> byteArray = GPJavaConvertor::ByteArrayFromTexture(CoverImage);
		jArray = GPJavaConvertor::ConvertToJByteArray(byteArray);
	}
	
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "writeSnapshot", "(Landroid/app/Activity;Lcom/ninevastudios/googleplay/GPSnapshot;[BLjava/lang/String;JJ[BII)V", FJavaWrapper::GameActivityThis, Snapshot->GetSnapshotObject(), GPJavaConvertor::ToJavaByteArray(Data), GPJavaConvertor::GetJavaString(Description), PlayedTimeMillis, ProgressValue, jArray, Width, Height);
#endif
}

void UGPCloudSaveLibrary::ReadSnapshot(UGPSnapshot* Snapshot, const FGPOnSnapshotReadSuccessDelegate& OnSnapshotReadSuccess, const FGPOnErrorDelegate& OnReadSnapshotFailure)
{
	OnSnapshotReadSuccessDelegate = OnSnapshotReadSuccess;
	OnReadSnapshotFailureDelegate = OnReadSnapshotFailure;

#if PLATFORM_ANDROID

	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "readSnapshot", "(Lcom/ninevastudios/googleplay/GPSnapshot;)V", Snapshot->GetSnapshotObject());
#endif
}

void UGPCloudSaveLibrary::GetMaxCoverImageSize(const FGPOnGetMaxSizeDelegate& OnGetMaxCoverImageSizeSuccess, const FGPOnErrorDelegate& OnGetMaxCoverImageFailure)
{
	UGPCloudSaveLibrary::OnGetMaxCoverImageSizeSuccessDelegate = OnGetMaxCoverImageSizeSuccess;
	UGPCloudSaveLibrary::OnGetMaxCoverImageFailureDelegate = OnGetMaxCoverImageFailure;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "getMaxCoverImageSize", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis);
#endif
}

void UGPCloudSaveLibrary::GetMaxDataSize(const FGPOnGetMaxSizeDelegate& OnGetMaxDataSizeSuccess, const FGPOnErrorDelegate& OnGetMaxDataFailure)
{
	UGPCloudSaveLibrary::OnGetMaxDataSizeSuccessDelegate = OnGetMaxDataSizeSuccess;
	UGPCloudSaveLibrary::OnGetMaxDataFailureDelegate = OnGetMaxDataFailure;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "getMaxDataSize", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis);
#endif
}

void UGPCloudSaveLibrary::DeleteSnapshot(UGPSnapshotMetadata* SnapshotMetadata, const FGPOnDeleteSnapshotDelegate& OnDeleteSnapshotSuccess, const FGPOnErrorDelegate& OnDeleteSnapshotFailure)
{
	UGPCloudSaveLibrary::OnDeleteSnapshotSuccessDelegate = OnDeleteSnapshotSuccess;
	UGPCloudSaveLibrary::OnDeleteSnapshotFailureDelegate = OnDeleteSnapshotFailure;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "deleteSnapshot", "(Landroid/app/Activity;Lcom/ninevastudios/googleplay/GPSnapshotMetadata;)V", FJavaWrapper::GameActivityThis, SnapshotMetadata->GetSnapshotMetadataObject());
#endif

}

void UGPCloudSaveLibrary::LoadSnapshotMetadata(bool ForceReload, const FGPOnLoadSnapshotMetadataDelegate& OnLoadSnapshotMetadata, const FGPOnErrorDelegate& OnLoadSnapshotMetadataFailure)
{
	UGPCloudSaveLibrary::OnLoadSnapshotMetadataDelegate = OnLoadSnapshotMetadata;
	UGPCloudSaveLibrary::OnLoadSnapshotMetadataFailureDelegate = OnLoadSnapshotMetadataFailure;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "loadSnapshot", "(Landroid/app/Activity;Z)V", FJavaWrapper::GameActivityThis, ForceReload);
#endif
}

void UGPCloudSaveLibrary::ResolveConflict(const FString& ConflictId, UGPSnapshot* Snapshot, const FGPOnSnapshotOpenSuccessDelegate& OnResolveConflictSuccess, const FGPOnErrorDelegate& OnResolveConflictFailure, const FGPOnConflictDelegate& OnResolveConflictConflict)
{
	UGPCloudSaveLibrary::OnResolveConflictSuccessDelegate = OnResolveConflictSuccess;
	UGPCloudSaveLibrary::OnResolveConflictFailureDelegate = OnResolveConflictFailure;
	UGPCloudSaveLibrary::OnResolveConflictConflictDelegate = OnResolveConflictConflict;

#if PLATFORM_ANDROID
	GPMethodCallUtils::CallStaticVoidMethod("com/ninevastudios/googleplay/GPCloudSaveLibrary", "resolveConflict", "(Landroid/app/Activity;Ljava/lang/String;Lcom/ninevastudios/googleplay/GPSnapshot;)V", FJavaWrapper::GameActivityThis, GPJavaConvertor::GetJavaString(ConflictId), Snapshot->GetSnapshotObject());
#endif
}

#if PLATFORM_ANDROID

// Show Saved Games UI

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onCloudSaveCancelCallback(JNIEnv* env, jclass clazz)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnCloudSaveCanceledDelegate.ExecuteIfBound();
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onCloudSaveErrorCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnCloudSaveErrorDelegate.ExecuteIfBound(ErrorMessage);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onAddNewSnapshotCallback(JNIEnv* env, jclass clazz)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnAddNewSnapshotDelegate.ExecuteIfBound();
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPIntermediateActivity_onChooseSnapshotCallback(JNIEnv* env, jclass clazz, jobject SnapshotMetadataObject)
{
	UGPSnapshotMetadata* SnapshotMetadata = NewObject<UGPSnapshotMetadata>();
	SnapshotMetadata->Init(SnapshotMetadataObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnViewSnapshotMetadataDelegate.ExecuteIfBound(SnapshotMetadata);
	});
}

// Open Snapshot With File Name

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onOpenSnapshotByFileNameSuccessCallback(JNIEnv* env, jclass clazz, jobject snapshotObject)
{
	UGPSnapshot* Snapshot = NewObject<UGPSnapshot>();
	Snapshot->Init(snapshotObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnSnapshotOpenWithFileNameSuccessDelegate.ExecuteIfBound(Snapshot);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onOpenSnapshotByFileNameFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnOpenSnapshotWithFileNameErrorDelegate.ExecuteIfBound(ErrorMessage);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onOpenSnapshotByFileNameConflictCallback(JNIEnv* env, jclass clazz, jobject conflictObject)
{
	UGPConflict* Conflict = NewObject<UGPConflict>();
	Conflict->Init(conflictObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnOpenSnapshotByFileNameConflictDelegate.ExecuteIfBound(Conflict);
	});
}

// Write Snapshot

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onWriteSnapshotSuccessCallback(JNIEnv* env, jclass clazz, jobject snapshotMetadataObject)
{
	UGPSnapshotMetadata* SnapshotMetadata = NewObject<UGPSnapshotMetadata>();
	SnapshotMetadata->Init(snapshotMetadataObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnWriteSnapshotSuccessDelegate.ExecuteIfBound(SnapshotMetadata);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onWriteSnapshotFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnWriteSnapshotFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

// Open Snapshot With Metadata
JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onOpenSnapshotWithMetadataSuccessCallback(JNIEnv* env, jclass clazz, jobject snapshotObject)
{
	UGPSnapshot* Snapshot = NewObject<UGPSnapshot>();
	Snapshot->Init(snapshotObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnSnapshotOpenWithMetadataSuccessDelegate.ExecuteIfBound(Snapshot);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onOpenSnapshotWithMetadataFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnOpenSnapshotWithMetadataErrorDelegate.ExecuteIfBound(ErrorMessage);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onOpenSnapshotWithMetadataConflictCallback(JNIEnv* env, jclass clazz, jobject conflictObject)
{
	UGPConflict* Conflict = NewObject<UGPConflict>();
	Conflict->Init(conflictObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnOpenSnapshotWithMetadataConflictDelegate.ExecuteIfBound(Conflict);
	});
}

//Read Snapshot

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onReadSnapshotSuccessCallback(JNIEnv* env, jclass clazz, jbyteArray snapshotDataArray)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	TArray<uint8> dataArray = GPJavaConvertor::ToByteArray(snapshotDataArray);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnSnapshotReadSuccessDelegate.ExecuteIfBound(dataArray);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onReadSnapshotFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnReadSnapshotFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

// Get Max Cover Image Size

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onGetMaxCoverImageSizeSuccessCallback(JNIEnv* env, jclass clazz, int maxCoverImageSize)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnGetMaxCoverImageSizeSuccessDelegate.ExecuteIfBound(maxCoverImageSize);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onGetMaxCoverImageSizeErrorCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnGetMaxCoverImageFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

// Get Max Data Size

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onGetMaxDataSizeSuccessCallback(JNIEnv* env, jclass clazz, int maxDataSize)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnGetMaxDataSizeSuccessDelegate.ExecuteIfBound(maxDataSize);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onGetMaxDataSizeFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnGetMaxDataFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

//Delete Snapshot

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onDeleteSnapshotMetadataSuccessCallback(JNIEnv* env, jclass clazz, jstring deleteMessage)
{
	FString DeleteMessage = FJavaHelper::FStringFromParam(env, deleteMessage);
	UE_LOG(LogMobileHelper, Warning, TEXT("%s"), *DeleteMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnDeleteSnapshotSuccessDelegate.ExecuteIfBound(DeleteMessage);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onDeleteSnapshotMetadataFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnGetMaxDataFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

//Load Snapshot

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onLoadSnapshotSuccessCallback(JNIEnv* env, jclass clazz, jobjectArray snapshotMetadataArray)
{
	TArray<UGPSnapshotMetadata*> SnapshotMetadataParsedArray;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	int length = Env->GetArrayLength(snapshotMetadataArray);
	for (int i = 0; i < length; i++)
	{
		auto javaSnapshotMetadata = Env->GetObjectArrayElement(snapshotMetadataArray, i);

		UGPSnapshotMetadata* SnapshotMetadata = NewObject<UGPSnapshotMetadata>();
		SnapshotMetadata->Init(javaSnapshotMetadata);
		SnapshotMetadataParsedArray.Add(SnapshotMetadata);
	}
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnLoadSnapshotMetadataDelegate.ExecuteIfBound(SnapshotMetadataParsedArray);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onLoadSnapshotFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnLoadSnapshotMetadataFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

// Resolve Conflict

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onResolveConflictSuccessCallback(JNIEnv* env, jclass clazz, jobject snapshotObject)
{
	UGPSnapshot* Snapshot = NewObject<UGPSnapshot>();
	Snapshot->Init(snapshotObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnResolveConflictSuccessDelegate.ExecuteIfBound(Snapshot);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onResolveConflictFailureCallback(JNIEnv* env, jclass clazz, jstring errorMessage)
{
	FString ErrorMessage = FJavaHelper::FStringFromParam(env, errorMessage);
	UE_LOG(LogMobileHelper, Error, TEXT("%s"), *ErrorMessage);

	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnResolveConflictFailureDelegate.ExecuteIfBound(ErrorMessage);
	});
}

JNI_METHOD void Java_com_ninevastudios_googleplay_GPCloudSaveLibrary_onResolveConflictConflictCallback(JNIEnv* env, jclass clazz, jobject conflictObject)
{
	UGPConflict* Conflict = NewObject<UGPConflict>();
	Conflict->Init(conflictObject);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		UGPCloudSaveLibrary::OnResolveConflictConflictDelegate.ExecuteIfBound(Conflict);
	});
}
#endif