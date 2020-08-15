import("core.platform.platform")
import("core.base.option")

function main(target)
    if target:isphony() then
        return 
    end
    
    local targetname = target:name()
    if "core" ~= targetname then
        return
    end

    local installdir = option.get("installdir") or os.getenv("INSTALLDIR") or os.getenv("DESTDIR") or platform.get("installdir")
    
    if is_plat("windows") then
        os.exec("xcopy /S /Q /Y /I hikyuu %s", os.args(installdir))
    end
    if is_plat("linux") then
        os.exec("cp -f -r -T hikyuu %s", os.args(installdir))
    end
    if is_plat("macosx") then
        os.exec("cp -f -r hikyuu/* %s", os.args(installdir))
    end
end