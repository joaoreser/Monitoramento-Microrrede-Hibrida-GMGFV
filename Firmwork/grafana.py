{
  "annotations": {
    "list": [
      {
        "builtIn": 1,
        "datasource": {
          "type": "grafana",
          "uid": "-- Grafana --"
        },
        "enable": true,
        "hide": true,
        "iconColor": "rgba(0, 211, 255, 1)",
        "name": "Annotations & Alerts",
        "type": "dashboard"
      }
    ]
  },
  "editable": true,
  "fiscalYearStartMonth": 0,
  "graphTooltip": 0,
  "id": 2,
  "links": [],
  "panels": [
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "fixedColor": "yellow",
            "mode": "fixed"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "watt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 5,
        "x": 0,
        "y": 0
      },
      "id": 17,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "P_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência Ativa SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "fixedColor": "#00ffd7",
            "mode": "fixed"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "watt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 5,
        "x": 5,
        "y": 0
      },
      "id": 7,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_p_ativa",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Potência Ativa GMG",
          "range": true,
          "refId": "B",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "thresholds"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "watt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 5,
        "x": 10,
        "y": 0
      },
      "id": 3,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "potencia_saida",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência de Saída FV",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "fixedColor": "#5b4dfb",
            "mode": "fixed"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "volt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 4,
        "x": 15,
        "y": 0
      },
      "id": 13,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Vae_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Tensão do SUPERCAP SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "thresholds"
          },
          "mappings": [],
          "thresholds": {
            "mode": "percentage",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "rothz"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 5,
        "x": 19,
        "y": 0
      },
      "id": 11,
      "options": {
        "colorMode": "background",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "value_and_name",
        "wideLayout": false
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Freq_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Frequência SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "volt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 2,
        "x": 0,
        "y": 7
      },
      "id": 1,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "exemplar": false,
          "expr": "Va_SIC",
          "format": "time_series",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "instant": false,
          "interval": "",
          "legendFormat": "Tensão Fase A SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Vb_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão Fase B SIC",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Vc_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão Fase C SIC",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "custom": {
            "axisBorderShow": false,
            "axisCenteredZero": false,
            "axisColorMode": "text",
            "axisLabel": "",
            "axisPlacement": "left",
            "barAlignment": 0,
            "barWidthFactor": 0.6,
            "drawStyle": "line",
            "fillOpacity": 0,
            "gradientMode": "opacity",
            "hideFrom": {
              "legend": false,
              "tooltip": false,
              "viz": false
            },
            "insertNulls": false,
            "lineInterpolation": "smooth",
            "lineStyle": {
              "fill": "solid"
            },
            "lineWidth": 2,
            "pointSize": 8,
            "scaleDistribution": {
              "type": "linear"
            },
            "showPoints": "always",
            "spanNulls": false,
            "stacking": {
              "group": "A",
              "mode": "none"
            },
            "thresholdsStyle": {
              "mode": "off"
            }
          },
          "fieldMinMax": false,
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "volt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 10,
        "x": 2,
        "y": 7
      },
      "id": 15,
      "options": {
        "legend": {
          "calcs": [],
          "displayMode": "table",
          "placement": "bottom",
          "showLegend": true
        },
        "tooltip": {
          "mode": "single",
          "sort": "none"
        }
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "exemplar": false,
          "expr": "Va_SIC",
          "format": "time_series",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "instant": false,
          "interval": "",
          "legendFormat": "Tensão Fase A SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Vb_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão Fase B SIC",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Vc_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão Fase C SIC",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "timeseries"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              },
              {
                "color": "red",
                "value": 80
              }
            ]
          },
          "unit": "amp"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 2,
        "x": 12,
        "y": 7
      },
      "id": 16,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Ia_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Corrente RMS Fase A SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Ib_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase B SIC",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Ic_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase C SIC",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "fixedColor": "orange",
            "mode": "palette-classic",
            "seriesBy": "max"
          },
          "custom": {
            "axisBorderShow": false,
            "axisCenteredZero": false,
            "axisColorMode": "text",
            "axisLabel": "",
            "axisPlacement": "auto",
            "barAlignment": 0,
            "barWidthFactor": 0.6,
            "drawStyle": "line",
            "fillOpacity": 0,
            "gradientMode": "opacity",
            "hideFrom": {
              "legend": false,
              "tooltip": false,
              "viz": false
            },
            "insertNulls": false,
            "lineInterpolation": "smooth",
            "lineStyle": {
              "fill": "solid"
            },
            "lineWidth": 2,
            "pointSize": 10,
            "scaleDistribution": {
              "type": "linear"
            },
            "showPoints": "always",
            "spanNulls": false,
            "stacking": {
              "group": "A",
              "mode": "none"
            },
            "thresholdsStyle": {
              "mode": "off"
            }
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "amp"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 10,
        "x": 14,
        "y": 7
      },
      "id": 10,
      "options": {
        "legend": {
          "calcs": [],
          "displayMode": "table",
          "placement": "bottom",
          "showLegend": true
        },
        "tooltip": {
          "mode": "single",
          "sort": "none"
        }
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Ia_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Corrente RMS Fase A SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Ib_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase B SIC",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Ic_SIC",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase C SIC",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "timeseries"
    },
    {
      "datasource": {
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "thresholds"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              },
              {
                "color": "red",
                "value": 80
              }
            ]
          },
          "unit": "bool_on_off"
        },
        "overrides": [
          {
            "matcher": {
              "id": "byName",
              "options": "{__name__=\"i_rms\", instance=\"172.30.248.122:8000\", job=\"spi_prometheus\"}"
            },
            "properties": []
          }
        ]
      },
      "gridPos": {
        "h": 8,
        "w": 3,
        "x": 0,
        "y": 17
      },
      "id": 2,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "status_inversor",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Status do inversor FV",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "thresholds"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "watt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 8,
        "w": 4,
        "x": 8,
        "y": 17
      },
      "id": 5,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "potencia_ref",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência de Referência FV",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "thresholds"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              }
            ]
          },
          "unit": "volt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 8,
        "w": 2,
        "x": 12,
        "y": 17
      },
      "id": 20,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "tensao_paineis",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Tensão Painéis ",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "custom": {
            "axisBorderShow": false,
            "axisCenteredZero": false,
            "axisColorMode": "text",
            "axisLabel": "",
            "axisPlacement": "auto",
            "barAlignment": 0,
            "barWidthFactor": 0.6,
            "drawStyle": "line",
            "fillOpacity": 0,
            "gradientMode": "none",
            "hideFrom": {
              "legend": false,
              "tooltip": false,
              "viz": false
            },
            "insertNulls": false,
            "lineInterpolation": "smooth",
            "lineWidth": 2,
            "pointSize": 10,
            "scaleDistribution": {
              "type": "linear"
            },
            "showPoints": "always",
            "spanNulls": false,
            "stacking": {
              "group": "A",
              "mode": "none"
            },
            "thresholdsStyle": {
              "mode": "off"
            }
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green",
                "value": null
              },
              {
                "color": "red",
                "value": 80
              }
            ]
          }
        },
        "overrides": []
      },
      "gridPos": {
        "h": 8,
        "w": 10,
        "x": 14,
        "y": 17
      },
      "id": 4,
      "options": {
        "legend": {
          "calcs": [],
          "displayMode": "list",
          "placement": "bottom",
          "showLegend": true
        },
        "tooltip": {
          "mode": "single",
          "sort": "none"
        }
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "tensao_paineis",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Tensão Painéis ",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "timeseries"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "volt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 2,
        "x": 0,
        "y": 25
      },
      "id": 21,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_va",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Tensão RMS Fase A GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_vb",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão RMS Fase B GMG",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_vc",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão RMS Fase C GMG",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "custom": {
            "axisBorderShow": false,
            "axisCenteredZero": false,
            "axisColorMode": "text",
            "axisLabel": "",
            "axisPlacement": "auto",
            "barAlignment": 0,
            "barWidthFactor": 0.6,
            "drawStyle": "line",
            "fillOpacity": 0,
            "gradientMode": "none",
            "hideFrom": {
              "legend": false,
              "tooltip": false,
              "viz": false
            },
            "insertNulls": false,
            "lineInterpolation": "smooth",
            "lineWidth": 2,
            "pointSize": 9,
            "scaleDistribution": {
              "type": "linear"
            },
            "showPoints": "always",
            "spanNulls": false,
            "stacking": {
              "group": "A",
              "mode": "none"
            },
            "thresholdsStyle": {
              "mode": "off"
            }
          },
          "mappings": [],
          "min": 200,
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "volt"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 10,
        "x": 2,
        "y": 25
      },
      "id": 6,
      "options": {
        "legend": {
          "calcs": [],
          "displayMode": "table",
          "placement": "bottom",
          "showLegend": true
        },
        "tooltip": {
          "mode": "single",
          "sort": "none"
        }
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_va",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Tensão RMS Fase A GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_vb",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão RMS Fase B GMG",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_vc",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Tensão RMS Fase C GMG",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "timeseries"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "amp"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 2,
        "x": 12,
        "y": 25
      },
      "id": 22,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_ia",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Corrente RMS Fase A GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_ib",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase B GMG",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_ic",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase C GMG",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "palette-classic"
          },
          "custom": {
            "axisBorderShow": false,
            "axisCenteredZero": false,
            "axisColorMode": "text",
            "axisLabel": "",
            "axisPlacement": "auto",
            "barAlignment": 0,
            "barWidthFactor": 0.6,
            "drawStyle": "line",
            "fillOpacity": 0,
            "gradientMode": "none",
            "hideFrom": {
              "legend": false,
              "tooltip": false,
              "viz": false
            },
            "insertNulls": false,
            "lineInterpolation": "smooth",
            "lineWidth": 2,
            "pointSize": 10,
            "scaleDistribution": {
              "type": "linear"
            },
            "showPoints": "always",
            "spanNulls": false,
            "stacking": {
              "group": "A",
              "mode": "none"
            },
            "thresholdsStyle": {
              "mode": "off"
            }
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              },
              {
                "color": "red",
                "value": 80
              }
            ]
          }
        },
        "overrides": []
      },
      "gridPos": {
        "h": 10,
        "w": 10,
        "x": 14,
        "y": 25
      },
      "id": 9,
      "options": {
        "legend": {
          "calcs": [],
          "displayMode": "table",
          "placement": "bottom",
          "showLegend": true
        },
        "tooltip": {
          "mode": "single",
          "sort": "none"
        }
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_ia",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Corrente RMS Fase A GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_ib",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase B GMG",
          "range": true,
          "refId": "B",
          "useBackend": false
        },
        {
          "datasource": {
            "type": "prometheus",
            "uid": "ee8572bih7ri8f"
          },
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_ic",
          "fullMetaSearch": false,
          "hide": false,
          "includeNullMetadata": true,
          "instant": false,
          "legendFormat": "Corrente RMS Fase C GMG",
          "range": true,
          "refId": "C",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "timeseries"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "fixedColor": "purple",
            "mode": "fixed"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "voltamp"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 5,
        "x": 0,
        "y": 35
      },
      "id": 19,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "S_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência Aparente SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "fixedColor": "blue",
            "mode": "fixed"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              },
              {
                "color": "red",
                "value": 80
              }
            ]
          },
          "unit": "voltampreact"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 5,
        "x": 5,
        "y": 35
      },
      "id": 18,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "Q_SIC",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência Reativa SIC",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "fixed"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "voltampreact"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 6,
        "x": 10,
        "y": 35
      },
      "id": 23,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_p_reativa",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência Reativa GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "continuous-BlYlRd"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "voltamp"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 4,
        "x": 16,
        "y": 35
      },
      "id": 24,
      "options": {
        "colorMode": "value",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_p_aparente",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Potência Aparente GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    },
    {
      "datasource": {
        "type": "prometheus",
        "uid": "ee8572bih7ri8f"
      },
      "fieldConfig": {
        "defaults": {
          "color": {
            "mode": "thresholds"
          },
          "mappings": [],
          "thresholds": {
            "mode": "absolute",
            "steps": [
              {
                "color": "green"
              }
            ]
          },
          "unit": "rothz"
        },
        "overrides": []
      },
      "gridPos": {
        "h": 7,
        "w": 4,
        "x": 20,
        "y": 35
      },
      "id": 8,
      "options": {
        "colorMode": "background",
        "graphMode": "none",
        "justifyMode": "auto",
        "orientation": "auto",
        "percentChangeColorMode": "standard",
        "reduceOptions": {
          "calcs": [
            "lastNotNull"
          ],
          "fields": "",
          "values": false
        },
        "showPercentChange": false,
        "textMode": "auto",
        "wideLayout": true
      },
      "pluginVersion": "11.4.0",
      "targets": [
        {
          "disableTextWrap": false,
          "editorMode": "builder",
          "expr": "gmg_freq",
          "fullMetaSearch": false,
          "includeNullMetadata": true,
          "legendFormat": "Frequência GMG",
          "range": true,
          "refId": "A",
          "useBackend": false
        }
      ],
      "title": "",
      "transparent": true,
      "type": "stat"
    }
  ],
  "preload": false,
  "schemaVersion": 40,
  "tags": [],
  "templating": {
    "list": []
  },
  "time": {
    "from": "now-6h",
    "to": "now"
  },
  "timepicker": {},
  "timezone": "browser",
  "title": "Simulação_Monitoramento",
  "uid": "aeebxtgkdbfggf",
  "version": 40,
  "weekStart": ""
}