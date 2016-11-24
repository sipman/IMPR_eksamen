# IMPR_eksamen
*Morten Hartvigsens eksamens opgave til Imperativ Programmering på 1. semester, Aalborg Universitet.*

I den danske superliga mødes 12 fodboldhold indbyrdes i et antal kampe. Kampene er inddelt i runder med seks kampe i hver runde. (Dette var strukturen i superligaen i 2015-2016, og tidligere årgange). Der gives tre point for en sejr, ét point for uafgjort, og nul point for en tabt kamp. I tilfælde af pointlighed mellem to hold afgør målforskellen (differencen mellem antal mål scoret af holdet og antal mål scoret mod holdet), og dernæst det samlede antal scorede mål den indbyrdes placering af de to hold (sådan er reglerne i denne opgave). Hvis alt står lige, beslutter vi blot at ordne de to hold alfabetisk efter det forkortede klubnavn (som er på to eller tre store bogstaver).

Denne opgave handler om - på forskellig vis - at opgøre resultatet af superligaen 2015-2016. Du skal først indlæse samtlige kampresultater fra en [tekstfil](../blob/master/superliga-2015-2016) til et array af kampe, hvor hver kamp er en passende struct. Navnet på tekstfilen, som dit program indlæser, skal være "superliga-2015-2016". Kampresultaterne stammer fra www.superstats.dk. Undervejs i opgaven vil der endvidere være behov for at lave en struct, som samler forskellige egenskaber om et hold i superligaen.

Tekstfilen med kampresultaterne er linjeopdelt, med én kamp per linje. Formatet af en linje er følgende:
```
runde ugedag dato  klokkeslæt    hjemmehold - udehold   hjemmemål - udemål tilskuerantal
```
Runden angives som Rn, hvor n er et heltal mellem 1 og 33. Et eller flere mellemrum adskiller disse bestanddele i tekstfilen. Eksempelvis betyder linjen
```
R1 Fre     17/07/2015  18.00     FCN - SDR     0 - 2     3.211
```
at FCN spillede mod SDR i runde 1; FCN scorede ingen mål; SDR scorede to mål; Kampen blev spillet 17. juli 2015 kl 18.00 på FCNs hjemmebane. Kampen blev overværet af 3211 tilskuere.

På baggrund af de indlæste data om kampene skal du skrive et C program med følgende problemløsningsfunktioner:

1. En funktion som finder (og returnerer) et array af uafgjorte kampe, hvor der (i hver kamp) er scoret fire mål, eller flere.
2. En funktion som finder en evt. runde, hvor der er scoret mindre end 10 mål. Hvis runden findes skal både rundenummer og det antal mål, der i alt er scoret i denne runde, sendes tilbage til kaldstedet via parametre.
3. En funktion der finder (og returnerer) et array af de hold, som vinder flere kampe på udebane end på hjemmebane.
4. En funktion der finder det hold, som i 2015-kampe har haft færrest tilskuere ved kampe, som spilles på klubbens hjemmebane. Den funktion, som løser problemet, skal både sende holdnavn og tilskuertallet tilbage til kaldstedet via parametre.
5. En funktion der finder et array af kampe med starttidspunkt mellem to givne klokkeslæt k1 og k2 og som spilles på en bestemt ugedag u. Funktionen, som løser dette problem, skal have k1, k2 og u som input parametre (side om side med andre nødvendige parametre). Det kan antages at k1 og k2 er klokkeslæt i det samme døgn. Kampe som starter præcist på tidspunkterne k1 og k2 medtages. Klokkeslæt-parametre i din funktion angives som tekststrenge på formen "hh:mm", f.eks. "15:00". Ugedags-parametren angives i dit program på samme forkortede form som i tekstfilen. Det fundne array af kampe skal enten returneres med return, eller sendes tilbage til kaldstedet via en parameter.
6. En funktion der beregner og (på standard output) udskriver en samlet tabel over stillingen i turneringen efter sidste spillerunde, i stil med [stillingen vist i Wikipedias artikel](https://da.wikipedia.org/wiki/Superligaen_2015-16#Stilling) om superligaen 2015 - 2016. I tabellen skal holdene være ordnede efter placeringen i turneringen, med det bedst placerede hold først og det dårligst placerede hold sidst. Endvidere skal der udskrives 'søjler' med point, antal spillede kampe, antal vundne kampe, antal tabte kampe, antal uafgjorte kampe, og målforskel.

Skriv en main funktion, som i en simple tekst-orienteret dialog med brugeren tillader aktivering af ovennævnte funktioner. Det anbefales at denne del af programmet laves så simpel som mulig.

Lad os antage at dit oversatte program er på filen superliga.exe. Når dit program køres som
```
  superliga.exe --print
```
skal det - uden at modtage nogen form for input fra brugeren - udskrive dine resultater for alle seks opgaver på en klar og tydelig måde på standard output. (Brug programparametre til main for at opnå dette). I opgave 5 ønskes denne udskrift at omfatte søndagskampe som startes mellem kl. 13.15 og 14.15. Dette gøres ved at kalde den funktion, der programmeres i opgave 5, med parametre svarende til tidspunkterne "13:15" og "14:15" og ugedag "Son".

Når programmet startes uden angivelse af   --print   skal programmet starte med udførelse af den ønskede (simple) brugerdialog.

I denne opgave er det OK at anvende danske navne på variable og funktioner. Undgå dog, som sædvanlig, brugen af danske bogstaver (æ, ø, å, Æ, Ø, Å) i variabelnavne og funktionsnavne.

Når dit program skal vurderes vil der blive lagt vægt på følgende egenskaber:
+ Læsbarhed og forståelighed, herunder god og klar programopstilling (indrykning mv.) samt fornuftig anvendelse af programkommentarer.
+ God navngivning af variable og funktioner, der afspejler den rolle som variable og funktioner spiller i programmet.
+ Fornuftig nedbrydning af problemløsningen i et passende antal funktioner, som følge af top-down programmering ved trinvis forfinelse (del og hersk).
+ God parametrisering af funktionerne, således at al input til en funktion foregår gennem input parametre, og at output fra en funktionen formidles gennem returværdien eller output parametre.
+ Valg af hensigtsmæssige datastrukturer (structs og arrays).
+ Hensigtsmæssig anvendelse af funktioner fra C Standard Libraries.

Som følge af den begrænsede mængde af data, som skal behandles i denne opgave, prioriteres et enkelt og klart kildeprogram højere end effektivitet af beregningerne.

Evt. uklarheder i opgaveformuleringen forventes afklaret som en del af opgaveløsningen (gennem kommentarer i det afleverede program).
