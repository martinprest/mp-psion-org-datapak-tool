sudo kill `pgrep openocd`
#sudo openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c "program /tree/projects/github/psion-org-datapack-tool/firmware/build/datapack_tool.elf verify reset exit"
sudo openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c "program /home/martin/Github/mp-psion-org-datapak-tool/firmware/build/datapak_tool.elf verify reset exit"
