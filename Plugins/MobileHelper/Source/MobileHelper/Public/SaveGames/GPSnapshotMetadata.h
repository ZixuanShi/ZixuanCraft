// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif

#include "GPSnapshotMetadata.generated.h"

UCLASS(BlueprintType)
class MOBILEHELPER_API UGPSnapshotMetadata : public UObject
{
	GENERATED_BODY()

public:
#if PLATFORM_ANDROID
	void Init(jobject SnapshotMetadata);

	jobject GetSnapshotMetadataObject();
#endif

	UGPSnapshotMetadata() = default;
	virtual ~UGPSnapshotMetadata();

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	FString GetDescription();

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	FString GetDeviceName();

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	FString GetSnapshotId();

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	FString GetUniqueName();
	
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	FString GetCoverImageUri();

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	float GetCoverImageAspectRatio();

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	void GetLastModifiedTimestamp(int64& LastModifiedTimestamp);
	
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	void GetPlayedTime(int64& SnapshotPlayedTime);

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|SnapshotMetadata")
	void GetProgressValue(int64& ProgressValue);

private:
#if PLATFORM_ANDROID

	jobject SnapshotMetadata;
	jmethodID GetDescriptionMethod;
	jmethodID GetDeviceNameMethod;
	jmethodID GetSnapshotIdMethod;
	jmethodID GetUniqueNameMethod;
	jmethodID GetCoverImageUriMethod;
	jmethodID GetCoverImageAspectRatioMethod;
	jmethodID GetLastModifiedTimestampMethod;
	jmethodID GetPlayedTimeMethod;
	jmethodID GetProgressValueMethod;

#endif
};
