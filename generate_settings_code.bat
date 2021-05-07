@echo off

pip install xlrd jinja2 simplejson
python gen_settings\\params_xls_to_json.py
python gen_settings\\params_json_to_c_storage_access.py
python gen_settings\\params_json_to_c_tft_menu.py
python gen_settings\\params_json_to_java_android.py
python gen_settings\\params_json_to_c_wifi_menu.py
python gen_settings\\params_json_to_c_wifi_menu.py
python gen_settings\\params_json_to_md_settings_en.py
python gen_settings\\params_json_to_html_settings_en.py