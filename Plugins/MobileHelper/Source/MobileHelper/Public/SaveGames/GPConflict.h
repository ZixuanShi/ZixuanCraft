// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GPSnapshot.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif

#include "GPConflict.generated.h"

UCLASS(BlueprintType)
class MOBILEHELPER_API UGPConflict : public UObject
{
	GENERATED_BODY()

public:
	UGPConflict() = default;
	virtual ~UGPConflict();

#if PLATFORM_ANDROID
	void Init(jobject ConflictObject);

	jobject GetConflictObject();
#endif

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Conflict")
	FString GetSnapshotconflictId() const;

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Conflict")
	UGPSnapshot* GetLocalSnapshot() const;

	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Conflict")
	UGPSnapshot* GetServerSnapshot() const;

private:

#if PLATFORM_ANDROID
	jobject ConflictObject;
	jmethodID GetSnapshotConflictIdMethod;
	jmethodID GetLocalSnapshotMethod;
	jmethodID GetServerSnapshotMethod;
#endif
};
