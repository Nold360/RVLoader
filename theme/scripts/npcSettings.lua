--Code for PMS2 configuration

function initNPC()
    npcLedCount = NPC.getLedCount()
    npcLedMode  = NPC.getLedMode()

    menu = {"ledCount", "ledMode", "saveConfig"}
    npcLedColor = {}
    num = 0
    for i=1, npcLedCount do
      for c=0, 2 do
        npcLedColor[i+num+c] = 128
        table.insert(menu, string.format("npcLedColor_%u", i+num+c))
      end
      num = num + 2
    end
    npcSelectedEnum = enum(menu)
    npcSelected = npcSelectedEnum[1]
end

function drawNPC(onFocus)
    local colWidth = getDimensions()[1] - leftColumnWidth
    if onFocus then
        Gfx.drawRectangle(0, (npcSelected.id - 1) * lineHeight, colWidth, lineHeight, Gfx.RGBA8(0x1F, 0x22, 0x27, 0xFF));
    end
    menuSystem.reset()
    
    menuSystem.printLine("LED Count:", npcSelected.id)
    menuSystem.printLineValue(npcLedCount)

    menuSystem.printLine("LED Mode:", npcSelected.id)
    if npcLedMode == NPC.LED_MODE_RAINBOW then
        menuSystem.printLineValue("Rainbow", 2)
    else
        menuSystem.printLineValue("Static", 1)
    end

    ledNum = 0
    for i=1, npcLedCount do
        menuSystem.printLine(string.format("Color LED %u:", ledNum), npcSelected.id)
        menuSystem.printLine("Red:", npcSelected.id)
        menuSystem.printLineValue(npcLedColor[i+ledNum])
        menuSystem.printLine("Green:", npcSelected.id)
        menuSystem.printLineValue(npcLedColor[i+ledNum+1])
        menuSystem.printLine("Blue:", npcSelected.id)
        menuSystem.printLineValue(npcLedColor[i+ledNum+2])
        ledNum = ledNum + 2
    end

    menuSystem.printLine("Save config", npcSelected.id)
end

function handleNPC(onFocus)
    local down = Pad.gendown(0)
    local held = Pad.genheld(0)
    local curId = npcSelected.id

    if down.BUTTON_B then
        handlingLeftColumn = true
        return
    end

    if down.BUTTON_DOWN and curId < npcSelectedEnum.size then
        curId = curId + 1
    end

    if down.BUTTON_UP and curId > 1 then
        curId = curId - 1
    end

    if npcSelected == npcSelectedEnum.ledCount then
        if down.BUTTON_LEFT and ledCount > 0 then
            ledCount = ledCount - 1
        end
        if down.BUTTON_RIGHT and ledCount < 16 then
            ledCount = ledCount + 1
        end
    elseif npcSelected == npcSelectedEnum.ledMode then
        if down.BUTTON_LEFT or down.BUTTON_RIGHT then
            npcLedMode = 1 - npcLedMode
        end
    end

    if down.BUTTON_A then
        if npcSelected == npcSelectedEnum.saveConfig then
            NPC.setLedCount(npcLedCount)
            NPC.setLedMode(npcLedMode)
        end
    end

    npcSelected = npcSelectedEnum[curId]
end
