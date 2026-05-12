#include "ShopController.h"
#include "../Item/ItemBase.h"

namespace TextRPG
{
	void ShopController::Setup(UIManager* ui, PotionShop* potionShop, User* user, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_PotionShop = potionShop;
		m_User = user;
		m_GlobalStateMachine = globalSM;
		m_LocalStateMachine.ChangeState(EShopState::Main);

		m_UI->OnPotionShopActionSelected.AddListener([this](int choice) {
			switch (choice) {
			case 1: m_LocalStateMachine.PushState(EShopState::Buy); break;
			case 2: m_LocalStateMachine.PushState(EShopState::Sell); break;
			case 3: m_LocalStateMachine.PushState(EShopState::Craft); break;
			case 0: m_GlobalStateMachine->PopState(); m_UI->PrintMessage("Leaving the potion shop."); break;
			default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
			}
		});

		m_UI->OnShopBuyRequested.AddListener([this](int choice, int count) {
			if (choice == 0) { m_LocalStateMachine.PopState(); return; } // 구매 취소 시 로컬 상태 복귀
			_handleShopBuyLogic(choice, count);
		});

		m_UI->OnShopSellRequested.AddListener([this](int choice, int count) {
			if (choice == 0) { m_LocalStateMachine.PopState(); return; } // 판매 취소 시 로컬 상태 복귀
			_handleShopSellLogic(choice, count);
		});

		m_UI->OnShopCraftActionSelected.AddListener([this](int choice) {
			if (choice == 0) { m_LocalStateMachine.PopState(); return; } // 제작 취소 시 로컬 상태 복귀
			_handleShopCraftLogic(choice);
		});
	}

	void ShopController::Process()
	{
		switch (m_LocalStateMachine.GetCurrentState())
		{
		case EShopState::Main:
			m_UI->PromptPotionShopAction();
			break;
		case EShopState::Buy:
			m_UI->PromptShopBuyAction(m_PotionShop->GetStock(), m_User->GetInventory().GetGold());
			break;
		case EShopState::Sell:
			m_UI->PromptShopSellAction(m_User->GetInventory());
			break;
		case EShopState::Craft:
			m_UI->PromptShopCraftAction();
			break;
		}
	}

	void ShopController::_handleShopBuyLogic(int choice, int count)
	{
		Inventory& inven = m_User->GetInventory();

		if (choice <= 0 || count <= 0)
		{
			m_UI->PrintMessage("Invalid choice or quantity.");
			return;
		}

		const auto& stockItems = m_PotionShop->GetStock().GetAllItems();
		if (choice > stockItems.size())
		{
			m_UI->PrintMessage("Invalid choice. Please try again.");
			return;
		}

		auto it = stockItems.begin();
		std::advance(it, choice - 1);
		int itemID = it->second->GetID();

		if (itemID == 101)
		{
			m_UI->PrintMessage("\n[Notice] HP Potions cannot be bought with gold. Use Empty Potions.");
			int exchangeCount = 0;
			m_UI->GetInputs("How many Empty Potions (ID: 100) will you exchange? (0 to cancel): ", exchangeCount);

			if (exchangeCount > 0)
			{
				if (m_PotionShop->ExchangeEmptyPotion(exchangeCount, inven))
					m_UI->PrintMessage("Exchanged successfully. You received " + std::to_string(exchangeCount) + " HP Potion(s).");
				else
					m_UI->PrintMessage("Exchange failed. Not enough Empty Potions.");
			}
			return;
		}

		ItemBase* item = m_PotionShop->GetStock().FindItem(itemID);
		if (!item)
		{
			m_UI->PrintMessage("Invalid Item ID.");
			return;
		}

		if (m_PotionShop->BuyItem(itemID, count, inven))
			m_UI->PrintMessage("Successfully bought " + std::to_string(count) + " " + item->GetName() + "(s).");
		else
			m_UI->PrintMessage("Not enough gold to buy " + std::to_string(count) + " " + item->GetName() + "(s).");
	}

	void ShopController::_handleShopSellLogic(int choice, int count)
	{
		Inventory& inven = m_User->GetInventory();

		if (choice <= 0 || count <= 0)
		{
			m_UI->PrintMessage("Invalid choice or quantity.");
			return;
		}

		const auto& invenItems = inven.GetAllItems();
		if (choice > invenItems.size())
		{
			m_UI->PrintMessage("Invalid choice. Please try again.");
			return;
		}

		auto it = invenItems.begin();
		std::advance(it, choice - 1);
		int itemID = it->second->GetID();

		int earned = m_PotionShop->SellItem(itemID, count, inven);
		if (earned >= 0)
			m_UI->PrintMessage("Successfully sold. You earned " + std::to_string(earned) + "G.");
		else
			m_UI->PrintMessage("Sell failed. Check the Item ID or quantity.");
	}

	void ShopController::_handleShopCraftLogic(int choice)
	{
		std::vector<Recipe> recipes;
		if (choice == 1) {
			recipes = m_PotionShop->ShowAllRecipe();
		} else if (choice == 2) {
			std::string name = m_UI->GetStringInput("Enter recipe name: ");
			recipes = m_PotionShop->SearchRecipeByName(name);
		} else if (choice == 3) {
			std::string ingredient = m_UI->GetStringInput("Enter ingredient name: ");
			recipes = m_PotionShop->SearchRecipeByIngredient(ingredient);
		} else {
			m_UI->PrintMessage("Invalid choice. Please try again.");
			return;
		}

		m_UI->DisplayRecipes(recipes, m_PotionShop->GetStock());
		if (recipes.empty()) return;

		int recipeIdx = 0;
		m_UI->GetInputs("Enter recipe number to craft (0 to cancel): ", recipeIdx);
		if (recipeIdx > 0 && recipeIdx <= recipes.size()) {
			Recipe targetRecipe = recipes[recipeIdx - 1];
			if (m_PotionShop->CraftItem(targetRecipe, m_User->GetInventory())) {
				m_UI->PrintMessage("Successfully crafted: " + targetRecipe.GetName() + "!");
			} else {
				m_UI->PrintMessage("Crafting failed. Not enough ingredients.");
			}
		}
	}
}