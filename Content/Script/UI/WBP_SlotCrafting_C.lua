--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_SlotCrafting_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:IsItemValid(ItemStack)   
    return false
end

function M:OnTake()
    local num = self.CraftingMatrix:GetSizeInventory()
    for i = 0, num - 1 do
        self.CraftingMatrix:DecrStackSize(i, 1)
    end

    self.CraftingMatrix:UpdateCrafting()
end

return M
