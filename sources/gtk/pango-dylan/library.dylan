module: dylan-user
copyright: See LICENSE file in this distribution.

define library pango
  use dylan;
  use common-dylan;
  use c-ffi;
  use glib;
  use gobject;
  use cairo;
  use glib;

  export pango;
end library;

define module pango
  use dylan;
  use common-dylan;
  use c-ffi;
  use gobject-glue;
  use glib;
  use gobject;
  use cairo;
  use glib;

  export
    pango-version-string,
    pango-version-check,
    pango-version,
    pango-units-to-double,
    pango-units-from-double,
    pango-unichar-direction,
    pango-trim-string,
    pango-split-file-list,
    pango-skip-space,
    pango-shape,
    pango-script-get-sample-language,
    pango-script-for-unichar,
    pango-scan-word,
    pango-scan-string,
    pango-scan-int,
    pango-read-line,
    pango-quantize-line-geometry,
    pango-parse-weight,
    pango-parse-variant,
    pango-parse-style,
    pango-parse-stretch,
    pango-parse-markup,
    pango-parse-enum,
    pango-log2vis-get-embedding-levels,
    pango-itemize-with-base-dir,
    pango-itemize,
    pango-is-zero-width,
    pango-gravity-to-rotation,
    pango-gravity-get-for-script-and-width,
    pango-gravity-get-for-script,
    pango-gravity-get-for-matrix,
    pango-get-mirror-char,
    pango-get-log-attrs,
    pango-find-paragraph-boundary,
    pango-find-base-dir,
    pango-extents-to-pixels,
    pango-break,
    pango-bidi-type-for-unichar,
    pango-attr-type-register,
    pango-attr-type-get-name,
    pango-_script-for-lang-scripts-setter,
    pango-_script-for-lang-scripts,
    pango-_script-for-lang-lang-setter,
    pango-_script-for-lang-lang,
    <Pango_ScriptForLang>,
    <PangoWrapMode*>,
    <PangoWrapMode>,
    $pango-wrap-word-char,
    $pango-wrap-char,
    $pango-wrap-word,
    <PangoWeight*>,
    <PangoWeight>,
    $pango-weight-ultraheavy,
    $pango-weight-heavy,
    $pango-weight-ultrabold,
    $pango-weight-bold,
    $pango-weight-semibold,
    $pango-weight-medium,
    $pango-weight-normal,
    $pango-weight-book,
    $pango-weight-light,
    $pango-weight-ultralight,
    $pango-weight-thin,
    <PangoVariant*>,
    <PangoVariant>,
    $pango-variant-small-caps,
    $pango-variant-normal,
    <PangoUnderline*>,
    <PangoUnderline>,
    $pango-underline-error,
    $pango-underline-low,
    $pango-underline-double,
    $pango-underline-single,
    $pango-underline-none,
    $unknown-glyph-width,
    $unknown-glyph-height,
    pango-tab-array-set-tab,
    pango-tab-array-resize,
    pango-tab-array-get-tabs,
    pango-tab-array-get-tab,
    pango-tab-array-get-size,
    pango-tab-array-get-positions-in-pixels,
    pango-tab-array-free,
    pango-tab-array-copy,
    pango-tab-array-new,
    <PangoTabArray>,
    <PangoTabAlign*>,
    <PangoTabAlign>,
    $pango-tab-left,
    <PangoStyle*>,
    <PangoStyle>,
    $pango-style-italic,
    $pango-style-oblique,
    $pango-style-normal,
    <PangoStretch*>,
    <PangoStretch>,
    $pango-stretch-ultra-expanded,
    $pango-stretch-extra-expanded,
    $pango-stretch-expanded,
    $pango-stretch-semi-expanded,
    $pango-stretch-normal,
    $pango-stretch-semi-condensed,
    $pango-stretch-condensed,
    $pango-stretch-extra-condensed,
    $pango-stretch-ultra-condensed,
    pango-script-iter-next,
    pango-script-iter-get-range,
    pango-script-iter-free,
    <PangoScriptIter>,
    <PangoScript*>,
    <PangoScript>,
    $pango-script-lydian,
    $pango-script-lycian,
    $pango-script-carian,
    $pango-script-vai,
    $pango-script-ol-chiki,
    $pango-script-cham,
    $pango-script-saurashtra,
    $pango-script-sundanese,
    $pango-script-rejang,
    $pango-script-lepcha,
    $pango-script-kayah-li,
    $pango-script-nko,
    $pango-script-phags-pa,
    $pango-script-phoenician,
    $pango-script-cuneiform,
    $pango-script-balinese,
    $pango-script-unknown,
    $pango-script-kharoshthi,
    $pango-script-old-persian,
    $pango-script-syloti-nagri,
    $pango-script-tifinagh,
    $pango-script-glagolitic,
    $pango-script-buginese,
    $pango-script-new-tai-lue,
    $pango-script-ugaritic,
    $pango-script-tai-le,
    $pango-script-linear-b,
    $pango-script-shavian,
    $pango-script-osmanya,
    $pango-script-limbu,
    $pango-script-cypriot,
    $pango-script-braille,
    $pango-script-tagbanwa,
    $pango-script-buhid,
    $pango-script-hanunoo,
    $pango-script-tagalog,
    $pango-script-yi,
    $pango-script-canadian-aboriginal,
    $pango-script-tibetan,
    $pango-script-thai,
    $pango-script-thaana,
    $pango-script-telugu,
    $pango-script-tamil,
    $pango-script-syriac,
    $pango-script-sinhala,
    $pango-script-runic,
    $pango-script-oriya,
    $pango-script-old-italic,
    $pango-script-ogham,
    $pango-script-myanmar,
    $pango-script-mongolian,
    $pango-script-malayalam,
    $pango-script-latin,
    $pango-script-lao,
    $pango-script-khmer,
    $pango-script-katakana,
    $pango-script-kannada,
    $pango-script-hiragana,
    $pango-script-hebrew,
    $pango-script-hangul,
    $pango-script-han,
    $pango-script-gurmukhi,
    $pango-script-gujarati,
    $pango-script-greek,
    $pango-script-gothic,
    $pango-script-georgian,
    $pango-script-ethiopic,
    $pango-script-devanagari,
    $pango-script-deseret,
    $pango-script-cyrillic,
    $pango-script-coptic,
    $pango-script-cherokee,
    $pango-script-bopomofo,
    $pango-script-bengali,
    $pango-script-armenian,
    $pango-script-arabic,
    $pango-script-inherited,
    $pango-script-common,
    $pango-script-invalid-code,
    $scale,
    <PangoRendererPrivate>,
    pango-renderer-class-_pango-reserved4,
    pango-renderer-class-_pango-reserved3,
    pango-renderer-class-_pango-reserved2,
    pango-renderer-class-draw-glyph-item,
    pango-renderer-class-prepare-run,
    pango-renderer-class-end,
    pango-renderer-class-begin,
    pango-renderer-class-part-changed,
    pango-renderer-class-draw-glyph,
    pango-renderer-class-draw-trapezoid,
    pango-renderer-class-draw-shape,
    pango-renderer-class-draw-error-underline,
    pango-renderer-class-draw-rectangle,
    pango-renderer-class-draw-glyphs,
    pango-renderer-class-parent-class,
    <PangoRendererClass>,
    pango-renderer-set-matrix,
    pango-renderer-set-color,
    pango-renderer-part-changed,
    pango-renderer-get-matrix,
    pango-renderer-get-layout-line,
    pango-renderer-get-layout,
    pango-renderer-get-color,
    pango-renderer-draw-trapezoid,
    pango-renderer-draw-rectangle,
    pango-renderer-draw-layout-line,
    pango-renderer-draw-layout,
    pango-renderer-draw-glyphs,
    pango-renderer-draw-glyph-item,
    pango-renderer-draw-glyph,
    pango-renderer-draw-error-underline,
    pango-renderer-deactivate,
    pango-renderer-activate,
    <PangoRenderer*>,
    pango-renderer-priv,
    pango-renderer-matrix,
    pango-renderer-active-count,
    pango-renderer-strikethrough,
    pango-renderer-underline,
    pango-renderer-parent-instance,
    <PangoRenderer>,
    <PangoRenderPart*>,
    <PangoRenderPart>,
    $pango-render-part-strikethrough,
    $pango-render-part-underline,
    $pango-render-part-background,
    $pango-render-part-foreground,
    pango-rectangle-height-setter,
    pango-rectangle-height,
    pango-rectangle-width-setter,
    pango-rectangle-width,
    pango-rectangle-y-setter,
    pango-rectangle-y,
    pango-rectangle-x-setter,
    pango-rectangle-x,
    <PangoRectangle>,
    $render-type-none,
    pango-matrix-translate,
    pango-matrix-transform-rectangle,
    pango-matrix-transform-point,
    pango-matrix-transform-pixel-rectangle,
    pango-matrix-transform-distance,
    pango-matrix-scale,
    pango-matrix-rotate,
    pango-matrix-get-font-scale-factor,
    pango-matrix-free,
    pango-matrix-copy,
    pango-matrix-concat,
    pango-matrix-y0-setter,
    pango-matrix-y0,
    pango-matrix-x0-setter,
    pango-matrix-x0,
    pango-matrix-yy-setter,
    pango-matrix-yy,
    pango-matrix-yx-setter,
    pango-matrix-yx,
    pango-matrix-xy-setter,
    pango-matrix-xy,
    pango-matrix-xx-setter,
    pango-matrix-xx,
    <PangoMatrix>,
    pango-log-attr-is-word-boundary-setter,
    pango-log-attr-is-word-boundary,
    pango-log-attr-is-expandable-space-setter,
    pango-log-attr-is-expandable-space,
    pango-log-attr-backspace-deletes-character-setter,
    pango-log-attr-backspace-deletes-character,
    pango-log-attr-is-sentence-end-setter,
    pango-log-attr-is-sentence-end,
    pango-log-attr-is-sentence-start-setter,
    pango-log-attr-is-sentence-start,
    pango-log-attr-is-sentence-boundary-setter,
    pango-log-attr-is-sentence-boundary,
    pango-log-attr-is-word-end-setter,
    pango-log-attr-is-word-end,
    pango-log-attr-is-word-start-setter,
    pango-log-attr-is-word-start,
    pango-log-attr-is-cursor-position-setter,
    pango-log-attr-is-cursor-position,
    pango-log-attr-is-white-setter,
    pango-log-attr-is-white,
    pango-log-attr-is-char-break-setter,
    pango-log-attr-is-char-break,
    pango-log-attr-is-mandatory-break-setter,
    pango-log-attr-is-mandatory-break,
    pango-log-attr-is-line-break-setter,
    pango-log-attr-is-line-break,
    <PangoLogAttr>,
    pango-layout-line-x-to-index,
    pango-layout-line-unref,
    pango-layout-line-ref,
    pango-layout-line-index-to-x,
    pango-layout-line-get-x-ranges,
    pango-layout-line-get-pixel-extents,
    pango-layout-line-get-extents,
    pango-layout-line-resolved-dir-setter,
    pango-layout-line-resolved-dir,
    pango-layout-line-is-paragraph-start-setter,
    pango-layout-line-is-paragraph-start,
    pango-layout-line-runs-setter,
    pango-layout-line-runs,
    pango-layout-line-length-setter,
    pango-layout-line-length,
    pango-layout-line-start-index-setter,
    pango-layout-line-start-index,
    pango-layout-line-layout-setter,
    pango-layout-line-layout,
    <PangoLayoutLine>,
    pango-layout-iter-next-run,
    pango-layout-iter-next-line,
    pango-layout-iter-next-cluster,
    pango-layout-iter-next-char,
    pango-layout-iter-get-run-readonly,
    pango-layout-iter-get-run-extents,
    pango-layout-iter-get-run,
    pango-layout-iter-get-line-yrange,
    pango-layout-iter-get-line-readonly,
    pango-layout-iter-get-line-extents,
    pango-layout-iter-get-line,
    pango-layout-iter-get-layout-extents,
    pango-layout-iter-get-layout,
    pango-layout-iter-get-index,
    pango-layout-iter-get-cluster-extents,
    pango-layout-iter-get-char-extents,
    pango-layout-iter-get-baseline,
    pango-layout-iter-free,
    pango-layout-iter-copy,
    pango-layout-iter-at-last-line,
    <PangoLayoutIter>,
    <PangoLayoutClass>,
    pango-layout-xy-to-index,
    pango-layout-set-wrap,
    pango-layout-set-width,
    pango-layout-set-text,
    pango-layout-set-tabs,
    pango-layout-set-spacing,
    pango-layout-set-single-paragraph-mode,
    pango-layout-set-markup-with-accel,
    pango-layout-set-markup,
    pango-layout-set-justify,
    pango-layout-set-indent,
    pango-layout-set-height,
    pango-layout-set-font-description,
    pango-layout-set-ellipsize,
    pango-layout-set-auto-dir,
    pango-layout-set-attributes,
    pango-layout-set-alignment,
    pango-layout-move-cursor-visually,
    pango-layout-is-wrapped,
    pango-layout-is-ellipsized,
    pango-layout-index-to-pos,
    pango-layout-index-to-line-x,
    pango-layout-get-wrap,
    pango-layout-get-width,
    pango-layout-get-unknown-glyphs-count,
    pango-layout-get-text,
    pango-layout-get-tabs,
    pango-layout-get-spacing,
    pango-layout-get-size,
    pango-layout-get-single-paragraph-mode,
    pango-layout-get-pixel-size,
    pango-layout-get-pixel-extents,
    pango-layout-get-log-attrs-readonly,
    pango-layout-get-log-attrs,
    pango-layout-get-lines-readonly,
    pango-layout-get-lines,
    pango-layout-get-line-readonly,
    pango-layout-get-line-count,
    pango-layout-get-line,
    pango-layout-get-justify,
    pango-layout-get-iter,
    pango-layout-get-indent,
    pango-layout-get-height,
    pango-layout-get-font-description,
    pango-layout-get-extents,
    pango-layout-get-ellipsize,
    pango-layout-get-cursor-pos,
    pango-layout-get-context,
    pango-layout-get-character-count,
    pango-layout-get-baseline,
    pango-layout-get-auto-dir,
    pango-layout-get-attributes,
    pango-layout-get-alignment,
    pango-layout-copy,
    pango-layout-context-changed,
    pango-layout-new,
    <PangoLayout*>,
    <PangoLayout>,
    pango-language-get-default,
    pango-language-from-string,
    pango-language-to-string,
    pango-language-matches,
    pango-language-includes-script,
    pango-language-get-scripts,
    pango-language-get-sample-string,
    <PangoLanguage>,
    pango-item-split,
    pango-item-free,
    pango-item-copy,
    pango-item-new,
    pango-item-analysis-setter,
    pango-item-analysis,
    pango-item-num-chars-setter,
    pango-item-num-chars,
    pango-item-length-setter,
    pango-item-length,
    pango-item-offset-setter,
    pango-item-offset,
    <PangoItem>,
    <PangoGravityHint*>,
    <PangoGravityHint>,
    $pango-gravity-hint-line,
    $pango-gravity-hint-strong,
    $pango-gravity-hint-natural,
    <PangoGravity*>,
    <PangoGravity>,
    $pango-gravity-auto,
    $pango-gravity-west,
    $pango-gravity-north,
    $pango-gravity-east,
    $pango-gravity-south,
    pango-glyph-vis-attr-is-cluster-start-setter,
    pango-glyph-vis-attr-is-cluster-start,
    <PangoGlyphVisAttr>,
    pango-glyph-string-x-to-index,
    pango-glyph-string-set-size,
    pango-glyph-string-index-to-x,
    pango-glyph-string-get-width,
    pango-glyph-string-get-logical-widths,
    pango-glyph-string-free,
    pango-glyph-string-extents-range,
    pango-glyph-string-extents,
    pango-glyph-string-copy,
    pango-glyph-string-new,
    pango-glyph-string-space,
    pango-glyph-string-log-clusters-setter,
    pango-glyph-string-log-clusters,
    pango-glyph-string-glyphs-setter,
    pango-glyph-string-glyphs,
    pango-glyph-string-num-glyphs-setter,
    pango-glyph-string-num-glyphs,
    <PangoGlyphString>,
    pango-glyph-item-iter-prev-cluster,
    pango-glyph-item-iter-next-cluster,
    pango-glyph-item-iter-init-start,
    pango-glyph-item-iter-init-end,
    pango-glyph-item-iter-free,
    pango-glyph-item-iter-copy,
    pango-glyph-item-iter-end-char-setter,
    pango-glyph-item-iter-end-char,
    pango-glyph-item-iter-end-index-setter,
    pango-glyph-item-iter-end-index,
    pango-glyph-item-iter-end-glyph-setter,
    pango-glyph-item-iter-end-glyph,
    pango-glyph-item-iter-start-char-setter,
    pango-glyph-item-iter-start-char,
    pango-glyph-item-iter-start-index-setter,
    pango-glyph-item-iter-start-index,
    pango-glyph-item-iter-start-glyph-setter,
    pango-glyph-item-iter-start-glyph,
    pango-glyph-item-iter-text-setter,
    pango-glyph-item-iter-text,
    pango-glyph-item-iter-glyph-item-setter,
    pango-glyph-item-iter-glyph-item,
    <PangoGlyphItemIter>,
    pango-glyph-item-split,
    pango-glyph-item-letter-space,
    pango-glyph-item-get-logical-widths,
    pango-glyph-item-free,
    pango-glyph-item-copy,
    pango-glyph-item-glyphs-setter,
    pango-glyph-item-glyphs,
    pango-glyph-item-item-setter,
    pango-glyph-item-item,
    <PangoGlyphItem>,
    pango-glyph-info-attr-setter,
    pango-glyph-info-attr,
    pango-glyph-info-geometry-setter,
    pango-glyph-info-geometry,
    pango-glyph-info-glyph-setter,
    pango-glyph-info-glyph,
    <PangoGlyphInfo>,
    pango-glyph-geometry-y-offset-setter,
    pango-glyph-geometry-y-offset,
    pango-glyph-geometry-x-offset-setter,
    pango-glyph-geometry-x-offset,
    pango-glyph-geometry-width-setter,
    pango-glyph-geometry-width,
    <PangoGlyphGeometry>,
    pango-fontset-get-metrics,
    pango-fontset-get-font,
    pango-fontset-foreach,
    <PangoFontset*>,
    <PangoFontset>,
    pango-font-metrics-unref,
    pango-font-metrics-ref,
    pango-font-metrics-get-underline-thickness,
    pango-font-metrics-get-underline-position,
    pango-font-metrics-get-strikethrough-thickness,
    pango-font-metrics-get-strikethrough-position,
    pango-font-metrics-get-descent,
    pango-font-metrics-get-ascent,
    pango-font-metrics-get-approximate-digit-width,
    pango-font-metrics-get-approximate-char-width,
    <PangoFontMetrics>,
    <PangoFontMask*>,
    <PangoFontMask>,
    $pango-font-mask-gravity,
    $pango-font-mask-size,
    $pango-font-mask-stretch,
    $pango-font-mask-weight,
    $pango-font-mask-variant,
    $pango-font-mask-style,
    $pango-font-mask-family,
    pango-font-map-load-fontset,
    pango-font-map-load-font,
    pango-font-map-list-families,
    pango-font-map-create-context,
    <PangoFontMap*>,
    <PangoFontMap>,
    pango-font-family-list-faces,
    pango-font-family-is-monospace,
    pango-font-family-get-name,
    <PangoFontFamily*>,
    <PangoFontFamily>,
    pango-font-face-list-sizes,
    pango-font-face-is-synthesized,
    pango-font-face-get-face-name,
    pango-font-face-describe,
    <PangoFontFace*>,
    <PangoFontFace>,
    pango-font-description-from-string,
    pango-font-description-unset-fields,
    pango-font-description-to-string,
    pango-font-description-to-filename,
    pango-font-description-set-weight,
    pango-font-description-set-variant,
    pango-font-description-set-style,
    pango-font-description-set-stretch,
    pango-font-description-set-size,
    pango-font-description-set-gravity,
    pango-font-description-set-family-static,
    pango-font-description-set-family,
    pango-font-description-set-absolute-size,
    pango-font-description-merge-static,
    pango-font-description-merge,
    pango-font-description-hash,
    pango-font-description-get-weight,
    pango-font-description-get-variant,
    pango-font-description-get-style,
    pango-font-description-get-stretch,
    pango-font-description-get-size-is-absolute,
    pango-font-description-get-size,
    pango-font-description-get-set-fields,
    pango-font-description-get-gravity,
    pango-font-description-get-family,
    pango-font-description-free,
    pango-font-description-equal,
    pango-font-description-copy-static,
    pango-font-description-copy,
    pango-font-description-better-match,
    pango-font-description-new,
    <PangoFontDescription>,
    pango-font-get-metrics,
    pango-font-get-glyph-extents,
    pango-font-get-font-map,
    pango-font-describe-with-absolute-size,
    pango-font-describe,
    pango-font-descriptions-free,
    <PangoFont*>,
    <PangoFont>,
    <PangoEngineShape>,
    <PangoEngineLang>,
    <PangoEllipsizeMode*>,
    <PangoEllipsizeMode>,
    $pango-ellipsize-end,
    $pango-ellipsize-middle,
    $pango-ellipsize-start,
    $pango-ellipsize-none,
    $engine-type-shape,
    $engine-type-lang,
    <PangoDirection*>,
    <PangoDirection>,
    $pango-direction-neutral,
    $pango-direction-weak-rtl,
    $pango-direction-weak-ltr,
    $pango-direction-ttb-rtl,
    $pango-direction-ttb-ltr,
    $pango-direction-rtl,
    $pango-direction-ltr,
    <PangoCoverageLevel*>,
    <PangoCoverageLevel>,
    $pango-coverage-exact,
    $pango-coverage-approximate,
    $pango-coverage-fallback,
    $pango-coverage-none,
    pango-coverage-unref,
    pango-coverage-to-bytes,
    pango-coverage-set,
    pango-coverage-max,
    pango-coverage-get,
    <PangoCoverage>,
    <PangoContextClass>,
    pango-context-set-matrix,
    pango-context-set-language,
    pango-context-set-gravity-hint,
    pango-context-set-font-map,
    pango-context-set-font-description,
    pango-context-set-base-gravity,
    pango-context-set-base-dir,
    pango-context-load-fontset,
    pango-context-load-font,
    pango-context-list-families,
    pango-context-get-metrics,
    pango-context-get-matrix,
    pango-context-get-language,
    pango-context-get-gravity-hint,
    pango-context-get-gravity,
    pango-context-get-font-map,
    pango-context-get-font-description,
    pango-context-get-base-gravity,
    pango-context-get-base-dir,
    pango-context-new,
    <PangoContext*>,
    <PangoContext>,
    pango-color-to-string,
    pango-color-parse,
    pango-color-free,
    pango-color-copy,
    pango-color-blue-setter,
    pango-color-blue,
    pango-color-green-setter,
    pango-color-green,
    pango-color-red-setter,
    pango-color-red,
    <PangoColor>,
    <PangoBidiType*>,
    <PangoBidiType>,
    $pango-bidi-type-on,
    $pango-bidi-type-ws,
    $pango-bidi-type-s,
    $pango-bidi-type-b,
    $pango-bidi-type-bn,
    $pango-bidi-type-nsm,
    $pango-bidi-type-cs,
    $pango-bidi-type-an,
    $pango-bidi-type-et,
    $pango-bidi-type-es,
    $pango-bidi-type-en,
    $pango-bidi-type-pdf,
    $pango-bidi-type-rlo,
    $pango-bidi-type-rle,
    $pango-bidi-type-al,
    $pango-bidi-type-r,
    $pango-bidi-type-lro,
    $pango-bidi-type-lre,
    $pango-bidi-type-l,
    pango-attribute-init,
    pango-attribute-equal,
    pango-attribute-destroy,
    pango-attribute-end-index-setter,
    pango-attribute-end-index,
    pango-attribute-start-index-setter,
    pango-attribute-start-index,
    pango-attribute-klass-setter,
    pango-attribute-klass,
    <PangoAttribute>,
    <PangoAttrType*>,
    <PangoAttrType>,
    $pango-attr-gravity-hint,
    $pango-attr-gravity,
    $pango-attr-absolute-size,
    $pango-attr-strikethrough-color,
    $pango-attr-underline-color,
    $pango-attr-letter-spacing,
    $pango-attr-fallback,
    $pango-attr-scale,
    $pango-attr-shape,
    $pango-attr-rise,
    $pango-attr-strikethrough,
    $pango-attr-underline,
    $pango-attr-background,
    $pango-attr-foreground,
    $pango-attr-font-desc,
    $pango-attr-size,
    $pango-attr-stretch,
    $pango-attr-variant,
    $pango-attr-weight,
    $pango-attr-style,
    $pango-attr-family,
    $pango-attr-language,
    $pango-attr-invalid,
    pango-attr-string-value-setter,
    pango-attr-string-value,
    pango-attr-string-attr-setter,
    pango-attr-string-attr,
    <PangoAttrString>,
    pango-attr-size-absolute-setter,
    pango-attr-size-absolute,
    pango-attr-size-size-setter,
    pango-attr-size-size,
    pango-attr-size-attr-setter,
    pango-attr-size-attr,
    <PangoAttrSize>,
    pango-attr-shape-destroy-func-setter,
    pango-attr-shape-destroy-func,
    pango-attr-shape-copy-func-setter,
    pango-attr-shape-copy-func,
    pango-attr-shape-data-setter,
    pango-attr-shape-data,
    pango-attr-shape-logical-rect-setter,
    pango-attr-shape-logical-rect,
    pango-attr-shape-ink-rect-setter,
    pango-attr-shape-ink-rect,
    pango-attr-shape-attr-setter,
    pango-attr-shape-attr,
    <PangoAttrShape>,
    pango-attr-list-unref,
    pango-attr-list-splice,
    pango-attr-list-ref,
    pango-attr-list-insert-before,
    pango-attr-list-insert,
    pango-attr-list-filter,
    pango-attr-list-copy,
    pango-attr-list-change,
    pango-attr-list-new,
    <PangoAttrList>,
    pango-attr-language-value-setter,
    pango-attr-language-value,
    pango-attr-language-attr-setter,
    pango-attr-language-attr,
    <PangoAttrLanguage>,
    pango-attr-iterator-range,
    pango-attr-iterator-next,
    pango-attr-iterator-get-font,
    pango-attr-iterator-get-attrs,
    pango-attr-iterator-destroy,
    <PangoAttrIterator>,
    pango-attr-int-value-setter,
    pango-attr-int-value,
    pango-attr-int-attr-setter,
    pango-attr-int-attr,
    <PangoAttrInt>,
    pango-attr-font-desc-desc-setter,
    pango-attr-font-desc-desc,
    pango-attr-font-desc-attr-setter,
    pango-attr-font-desc-attr,
    <PangoAttrFontDesc>,
    pango-attr-float-value-setter,
    pango-attr-float-value,
    pango-attr-float-attr-setter,
    pango-attr-float-attr,
    <PangoAttrFloat>,
    pango-attr-color-color-setter,
    pango-attr-color-color,
    pango-attr-color-attr-setter,
    pango-attr-color-attr,
    <PangoAttrColor>,
    pango-attr-class-equal,
    pango-attr-class-destroy,
    pango-attr-class-copy,
    pango-attr-class-type-setter,
    pango-attr-class-type,
    <PangoAttrClass>,
    pango-analysis-extra-attrs-setter,
    pango-analysis-extra-attrs,
    pango-analysis-language-setter,
    pango-analysis-language,
    pango-analysis-script-setter,
    pango-analysis-script,
    pango-analysis-flags-setter,
    pango-analysis-flags,
    pango-analysis-gravity-setter,
    pango-analysis-gravity,
    pango-analysis-level-setter,
    pango-analysis-level,
    pango-analysis-font-setter,
    pango-analysis-font,
    pango-analysis-lang-engine-setter,
    pango-analysis-lang-engine,
    pango-analysis-shape-engine-setter,
    pango-analysis-shape-engine,
    <PangoAnalysis>,
    <PangoAlignment*>,
    <PangoAlignment>,
    $pango-align-right,
    $pango-align-center,
    $pango-align-left,
    $attr-index-from-text-beginning,
    $analysis-flag-centered-baseline;
end module;

define module pango-cairo
  use dylan;
  use common-dylan;
  use c-ffi;
  use gobject-glue;
  use glib;
  use gobject;
  use cairo;
  use pango;

  export
    pango-cairo-context-get-font-options,
    pango-cairo-context-get-resolution,
    pango-cairo-context-get-shape-renderer,
    pango-cairo-context-set-font-options,
    pango-cairo-context-set-resolution,
    pango-cairo-context-set-shape-renderer,
    pango-cairo-create-context,
    pango-cairo-create-layout,
    pango-cairo-error-underline-path,
    pango-cairo-font-get-scaled-font,
    pango-cairo-font-get-type,
    pango-cairo-font-map-create-context,
    pango-cairo-font-map-get-default,
    pango-cairo-font-map-get-font-type,
    pango-cairo-font-map-get-resolution,
    pango-cairo-font-map-get-type,
    pango-cairo-font-map-new,
    pango-cairo-font-map-new-for-font-type,
    pango-cairo-font-map-set-default,
    pango-cairo-font-map-set-resolution,
    pango-cairo-glyph-string-path,
    pango-cairo-layout-line-path,
    pango-cairo-layout-path,
    pango-cairo-show-error-underline,
    pango-cairo-show-glyph-item,
    pango-cairo-show-glyph-string,
    pango-cairo-show-layout,
    pango-cairo-show-layout-line,
    pango-cairo-update-context,
    pango-cairo-update-layout;
end module;
