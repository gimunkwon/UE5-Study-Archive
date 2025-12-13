

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Zombie_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;

class USoundBase;
class UParticleSystem;

class UZombie_HUD;

UCLASS()
class ZOMBIESURVIVOR_API AZombie_Player : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie_Player();
protected:
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual  float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	


protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* SpawnVoiceSound;
	
	// 체력 관련
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxHealth = 100.0f;
	float CurrentHealth;
	
	// UI 관련
	// 에디터에서 WBP_HUD를 넣어줄 틀
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZombie_HUD> HUDClass;
	
	// 실제 화면에 떠 있는 위젯(인스턴스)
	UPROPERTY()
	UZombie_HUD* ActiveHUD;
	
	// 게임 오버 위젯 클래스 (에디터 지정용)
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
	// 총구 화염 이펙트 (에디터에서 P_Explosion 넣을 예정)
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UParticleSystem* MuzzleEffect;
	
	// 탄약 설정
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	int32 MaxAmmo = 30;
	int32 CurrentAmmo;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;
	
	// 재장전
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* ReloadAction;

	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AZombie_Projectile> ProjectileClass;
	
	// 발사 사운드
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	USoundBase* FireSound;

protected:
	
	void Move(const FInputActionValue& Value);
	void RotateToCursor();
	void Fire(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);

};
