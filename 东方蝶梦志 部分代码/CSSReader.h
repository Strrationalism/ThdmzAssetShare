#pragma once
#include <string>
#include <memory>
#include <map>
namespace Basic {
		class CSSReader
		{
		public:
			class CSSLabel {
				friend class CSSReader;
			private:
				std::map<std::string, std::string> m_attrs;
			public:
				inline std::string GetString(const std::string& attr) {
					return m_attrs.at(attr);
				}
				inline int GetInt(const std::string& attr) {
					return std::atoi(GetString(attr).c_str());
				}
				inline double GetFloat(const std::string& attr) {
					return std::atof(GetString(attr).c_str());
				}
			};
		private:
			std::map<std::string, std::shared_ptr<CSSLabel>> m_labels;

		public:
			void LoadCSS(const std::string&);
			inline std::shared_ptr<CSSLabel>& GetLabel(const std::string& l) {
				return m_labels[l];
			}
			
			auto begin() {
				return m_labels.begin();
			}

			auto end() {
				return m_labels.end();
			}
		};
}
