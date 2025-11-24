#!/usr/bin/env bash

langs=(fr_FR pt_BR es_MX es_AR ru_RU zh_CN de_DE pl_PL ko_KR ja_JP)


potfile=i18n/confidant.pot
podir=i18n/po

sources=($(find src -name \*.cpp -o -name \*.hpp -! -name i18n.hpp))

xgettext -C \
         --from-code=UTF-8 \
         --keyword=_ \
         --keyword=_n:1,2 \
         --keyword=error:1 \
         --keyword=info:1 \
         --keyword=warn:1 \
         --keyword=warnextra:1 \
         --keyword=debug:1 \
         --keyword=trace:1 \
         --keyword=fatal:1 \
         --keyword=pretty:1 \
         --keyword=extra:1 \
         --no-git \
         --add-comments \
         --package-name=confidant \
         --package-version=$(cat .version) \
         -o ${potfile} ${sources[@]}

for language in ${langs[@]}
do
    pofile=${podir}/${language}.po
    if [ -r ${pofile} ]
    then
        msgmerge --verbose --suffix=".backup" --update ${pofile} ${potfile}
    else
        gt_args=(--input=${potfile}
                 --locale=${language}.UTF-8
                 --output=${pofile}
                 --no-translator)
    
        msginit ${gt_args[@]}
    fi

done