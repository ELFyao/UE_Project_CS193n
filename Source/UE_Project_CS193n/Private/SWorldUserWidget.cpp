// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	 
	FVector2D ScreenPosition;
	if (AttachActor==NULL)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Attached Actor no longer valid, remove widget");
		RemoveFromParent();
		
		return;
	}

	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachActor->GetActorLocation(), ScreenPosition))
	{
		float scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
