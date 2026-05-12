#pragma once
#include <map>
#include <vector>
#include <string>
#include "../Core/Singleton.h"
#include "../Item/ItemBase.h"
#include "../Item/Recipe.h"

namespace TextRPG
{
	/**
	 * @brief 게임 내 모든 정적 데이터(아이템, 레시피, 몬스터 스탯 등)를 로드하고 관리하는 싱글톤 매니저입니다.
	 */
	class DataManager : public Singleton<DataManager>
	{
	friend class Singleton<DataManager>;

	private:
		std::map<int, ItemBase*> m_ItemDB; // ItemID를 키로 가지는 원본 아이템 저장소
		std::vector<Recipe> m_RecipeDB;    // 전체 레시피 목록 저장소
		// std::map<int, MonsterData> m_MonsterDB; // 몬스터 데이터도 추가 가능

	public:
		/** @brief 게임 시작 시 모든 외부 데이터 파일(CSV, JSON)을 로드합니다. */
		void LoadAllData();

		/** 
		 * @brief ID에 해당하는 아이템의 새로운 복사본(Clone)을 생성하여 반환합니다. 
		 * @param itemID 찾을 아이템 ID
		 * @return 생성된 아이템 객체 포인터 (실패 시 nullptr)
		 */
		ItemBase* CreateItem(int itemID) const;

		/** @brief 로드된 모든 레시피 목록을 반환합니다. */
		const std::vector<Recipe>& GetAllRecipes() const { return m_RecipeDB; }

	private:
		DataManager() = default;
		~DataManager();

		// 내부 파싱용 함수들
		void _loadItemData(const std::string& filePath);
		void _loadRecipeData(const std::string& filePath);
	};
}