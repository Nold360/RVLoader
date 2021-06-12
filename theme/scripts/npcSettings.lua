--Code for PMS2 configuration

function initNPC()
    npcLedCount = NPC.getLedCount()
    npcLedMode  = NPC.getLedMode()

    menu = {"ledCount", "ledMode", "ledColor"}
    npcLedColor = {}
    for i=4, (npcLedCount*3)+3 do
        npcLedColor[i] = 128
        rowName = string.format("ledColor_%u", i-3)
        menu[i] = rowName
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

    ledNum = 1
    for i=1, (npcLedCount*3)+1, 3 do
        menuSystem.printLine(string.format("Color LED %u:", ledNum), npcSelected.id)
        menuSystem.printLine("Red:", npcSelected.id)
        menuSystem.printLineValue(ledColor[i])
        menuSystem.printLine("Green:", npcSelected.id)
        menuSystem.printLineValue(ledColor[i+1])
        menuSystem.printLine("Blue:", npcSelected.id)
        menuSystem.printLineValue(ledColor[i+2])
        ledNum = ledNum + 1
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
