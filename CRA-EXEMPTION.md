# CRA Exemption & Non-Liability Statement

**Version:** 1.0  
**Date:** August 2026  
**Applicable Regulation:** Regulation (EU) 2024/2847 (Cyber Resilience Act – CRA)

---

## 1. Project Status – Explicit Non-Commercial Exclusion

This repository and its source code (the “Project” / **`ESP32State`**) are developed and maintained exclusively by a **private individual** as a non-commercial, unpaid open-source initiative.

The Project is:
- Fully open-source (licensed under the permissive **MIT License**).
- Provided free of charge, without any commercial offering, paid support, subscriptions, or monetization.
- Developed outside the course of any commercial activity.
- Not “placed on the market” or “put into service” by a commercial manufacturer within the meaning of the CRA.

**Therefore, pursuant to Article 2(5) and Recital 10 of the CRA, this Project is explicitly excluded from the scope of the Regulation.**

---

## 2. Legal Basis for Open-Source Libraries

Unlike documentation-only repositories, `ESP32State` contains executable C++ source code (a C++17 Header-Only Library). The CRA explicitly regulates how open-source software developed by private individuals is treated:

### 2.1 The "Commercial Activity" Threshold
The CRA applies **only** to products with digital elements that are made available on the market *in the course of a commercial activity* (Art. 2(1)). 

**Recital 10** of the CRA explicitly states:
> *“Free and open-source software developed or supplied outside the course of a commercial activity should not be covered by this Regulation. This is particularly the case for software that is made available, including its source code and documentation, on open repositories and where there is no monetization or commercial intent.”*

### 2.2 Protection for Individual Open-Source Developers
**Recital 18** and **Recital 21** clarify the liability protection for upstream open-source contributors:
> *“[…] Developers contributing unpaid to open source are not subject to obligations under this Regulation, even if the software ends up in critical infrastructure.”*

---

## 3. Clarification on Downstream Responsibility & "The Manufacturer"

**Addressed to Enterprise Users & Legal Departments:**

If a commercial entity, company, or integrator uses `ESP32State` inside a commercial product (e.g., an Industrial IoT controller or medical device placed on the EU market), the CRA obligations do **not** cascade upstream to this repository or its author.

### 3.1 Who is the "Manufacturer"?
According to **Article 3(1)** of the CRA, a *Manufacturer* is a natural or legal person who develops a product **and places it on the market under their own name or trademark**. 
- The private author of `ESP32State` is **not** a manufacturer under the CRA.
- The commercial entity integrating this library into their commercial product **is** the sole manufacturer under the CRA.

### 3.2 Liability Allocation
The **sole legal responsibility** for:
- CRA Conformity Assessment (CE marking),
- Vulnerability handling & ENISA reporting (Art. 14),
- Creation of the Software Bill of Materials (SBOM), and
- Cyber resilience of the final product

rests **exclusively with the commercial integrator** placing the final product on the EU market.

---

## 4. Open Source Stewards (Art. 3(14))

`ESP32State` is maintained by an individual software developer, not an organizational entity. This repository does **not** fall under the definition of an "Open Source Software Steward" pursuant to Art. 3(14), as the maintainer does not act as a commercial intermediary, foundation, or legal entity providing systematic commercial ecosystem support.

---

## 5. What Downstream Commercial Users Must Do

If your company incorporates `ESP32State` into a commercial product destined for the European market, your internal compliance workflow should be:

1. **Conduct your own Risk Assessment:** Evaluate `ESP32State` as an upstream open-source C++ component within your specific application context.
2. **Include in SBOM:** List `ESP32State` as a third-party C++ library component in your product's Software Bill of Materials.
3. **Perform Audits on Your Side:** If your product requires specific security certifications (e.g., CRA Class I/II or NIS2 compliance), your engineering team must audit and validate the source code integrated into your firmware binary.

**You cannot and should not:**
- Require CE marking, formal declarations of conformity, or liability guarantees from this open-source repository.
- Require the maintainer to complete vendor security questionnaires or undergo third-party CRA audits.

---

## 6. Ready-to-Use Statement for Corporate Legal Departments (Copy & Paste)

If your legal or procurement department requests a formal CRA declaration regarding `ESP32State`, you may provide them with the following text:

---

**Subject: CRA Applicability Statement – ESP32State**

> *“Pursuant to Article 2(5) and Recitals 10 & 18 of Regulation (EU) 2024/2847 (Cyber Resilience Act), it is hereby confirmed that `ESP32State` is a non-commercial, unpaid open-source project maintained by a private individual. The source code is provided under the MIT License without commercial offering or paid support. Legally, the maintainer is not a 'Manufacturer' nor an 'Open Source Steward' under the CRA. Consequently, CE marking, formal conformity assessments, or ENISA reporting obligations do not apply to this repository. Full CRA compliance and product liability rest solely with the commercial entity that integrates this library into a product placed on the EU market.”*

---

## 7. References

- [Regulation (EU) 2024/2847 (Full Text)](https://eur-lex.europa.eu/legal-content/EN/TXT/?uri=CELEX%3A32024R2847)
- [EU Cyber Resilience Act – Open Source Guidance (European Commission)](https://digital-strategy.ec.europa.eu/en/policies/cra-open-source)

---

*This statement is provided for compliance transparency. It does not constitute formal legal advice. Downstream commercial users are advised to consult their legal counsel regarding CRA obligations for their finished products.*
